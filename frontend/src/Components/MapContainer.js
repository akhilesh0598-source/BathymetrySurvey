import React, { useState, useEffect, useRef } from 'react';
import OfflineMap from './OfflineMap';
import SensorData from './SensorData';
import config from '../config';
import { toast } from 'react-toastify';

const DEFAULT_DATA = {
  distance: 0,
  confidence: 0,
  latitude: 23.406855,
  longitude: 77.694556,
  gpsDateTime: '00:00:00',
  zoomVal: 3,
};

const haversineDistance = (lat1, lon1, lat2, lon2) => {
  const toRad = deg => (deg * Math.PI) / 180;
  const R = 6371000; // meters
  const dLat = toRad(lat2 - lat1);
  const dLon = toRad(lon2 - lon1);
  const a =
    Math.sin(dLat / 2) ** 2 +
    Math.cos(toRad(lat1)) * Math.cos(toRad(lat2)) *
    Math.sin(dLon / 2) ** 2;
  return R * 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
};

const MapContainer = () => {
  const [value, setValue] = useState(DEFAULT_DATA);
  const [isLoading, setIsLoading] = useState(false);
  const [resetPath, setResetPath] = useState(false);

  const socketRef = useRef(null);
  const reconnectTimeoutRef = useRef(null);
  const heartbeatIntervalRef = useRef(null);
  const reconnectAttempts = useRef(0);
  const prevCoordsRef = useRef({ lat: DEFAULT_DATA.latitude, lon: DEFAULT_DATA.longitude });
  const manualStopRef = useRef(false);

  const MAX_RECONNECT_DELAY = 30000; // max backoff 30 seconds

  const getHeartbeatInterval = () => {
    // If tab is visible, ping every 10 sec; if hidden, every 30 sec (less frequent)
    return document.visibilityState === 'visible' ? 10000 : 30000;
  };

  const startHeartbeat = () => {
    if (heartbeatIntervalRef.current) clearInterval(heartbeatIntervalRef.current);
    heartbeatIntervalRef.current = setInterval(() => {
      if (socketRef.current && socketRef.current.readyState === WebSocket.OPEN) {
        try {
          socketRef.current.send(JSON.stringify({ type: 'ping' }));
          //console.log('Ping sent');
        } catch (e) {
          console.warn('Ping send error:', e);
        }
      }
    }, getHeartbeatInterval());
  };

  const stopHeartbeat = () => {
    if (heartbeatIntervalRef.current) {
      clearInterval(heartbeatIntervalRef.current);
      heartbeatIntervalRef.current = null;
    }
  };

  const connectWebSocket = () => {
    if (socketRef.current) return; // Already connected or connecting

    setIsLoading(true);
    manualStopRef.current = false;

    const ws = new WebSocket(config.socketUrl);

    ws.onopen = () => {
      console.log("WebSocket connected");
      setIsLoading(false);
      reconnectAttempts.current = 0;
      socketRef.current = ws;
      startHeartbeat();
      toast.success("WebSocket connected");
      ws.send("Hello Server!");
    };

    ws.onmessage = (event) => {
      try {
        const parsedData = JSON.parse(event.data);
        if (parsedData.type === 'pong') {
          // Pong response for heartbeat ping
          //console.log('Pong received');
          return;
        }
        const { latitude, longitude } = parsedData;
        const prev = prevCoordsRef.current;
        const movedDistance = haversineDistance(prev.lat, prev.lon, latitude, longitude);
        prevCoordsRef.current = { lat: latitude, lon: longitude };

        let zoomVal = 16;
       
        if (movedDistance > 500) zoomVal = 10;
        else if (movedDistance > 200) zoomVal = 12;
        else if (movedDistance > 100) zoomVal = 14;
        else if (movedDistance > 50) zoomVal = 16;

        parsedData.zoomVal = zoomVal;
        setValue(parsedData);
      } catch (error) {
        console.error("Parsing error:", error);
        toast.error("Failed to parse incoming data");
      }
    };

    ws.onerror = (error) => {
      console.error("WebSocket error:", error);
      toast.error("WebSocket error");
      setIsLoading(false);
    };

    ws.onclose = (event) => {
      console.log("WebSocket closed", event);
      stopHeartbeat();
      socketRef.current = null;
      setIsLoading(false);
      setValue(DEFAULT_DATA);
      toast.info("WebSocket disconnected");

      if (!manualStopRef.current) {
        // Auto reconnect with backoff
        const delay = Math.min(1000 * 2 ** reconnectAttempts.current, MAX_RECONNECT_DELAY);
        console.log(`Reconnecting in ${delay}ms...`);
        reconnectTimeoutRef.current = setTimeout(() => {
          reconnectAttempts.current++;
          connectWebSocket();
        }, delay);
      }
    };
  };

  const handleStart = () => {
    setResetPath(true);
    prevCoordsRef.current = { lat: DEFAULT_DATA.latitude, lon: DEFAULT_DATA.longitude };
    if (reconnectTimeoutRef.current) {
      clearTimeout(reconnectTimeoutRef.current);
      reconnectTimeoutRef.current = null;
    }
    reconnectAttempts.current = 0;
    manualStopRef.current = false;
    connectWebSocket();
  };
const handleStop = () => {
  if (!socketRef.current) {
    toast.warn("WebSocket already stopped");
    return;
  }
  manualStopRef.current = true;

  // Clear reconnect timeout immediately
  if (reconnectTimeoutRef.current) {
    clearTimeout(reconnectTimeoutRef.current);
    reconnectTimeoutRef.current = null;
  }

  // Stop heartbeat interval immediately
  stopHeartbeat();

  // Remove event handlers so no auto reconnect triggers
  if (socketRef.current) {
    socketRef.current.onopen = null;
    socketRef.current.onmessage = null;
    socketRef.current.onerror = null;
    socketRef.current.onclose = null;

    // Close websocket
    socketRef.current.close();
  }

  // Clear socket ref immediately
  socketRef.current = null;

  // Update UI immediately
  setIsLoading(false);
  setResetPath(false);
  setValue(DEFAULT_DATA);

  toast.info("WebSocket stopped");
};


  // Handle page visibility changes
  useEffect(() => {
    const handleVisibilityChange = () => {
      if (socketRef.current && socketRef.current.readyState === WebSocket.OPEN) {
        stopHeartbeat();
        startHeartbeat();
      }
    };
    document.addEventListener('visibilitychange', handleVisibilityChange);

    return () => {
      document.removeEventListener('visibilitychange', handleVisibilityChange);
    };
  }, []);

  // Cleanup on unmount
  useEffect(() => {
    return () => {
      stopHeartbeat();
      if (reconnectTimeoutRef.current) clearTimeout(reconnectTimeoutRef.current);
      if (socketRef.current) socketRef.current.close();
      socketRef.current = null;
    };
  }, []);

  // Clear resetPath flag after reset
  useEffect(() => {
    if (resetPath) {
      const timeout = setTimeout(() => setResetPath(false), 100);
      return () => clearTimeout(timeout);
    }
  }, [resetPath]);

  return (
    <div className="container my-4">
      <div className="row">
        {/* Map Panel */}
        <div className="col-12 col-md-9 mb-3 mb-md-0 d-flex flex-column">
          <div className="flex-grow-1 border rounded shadow-sm overflow-hidden">
            <OfflineMap gpsData={value} isTracking={!!socketRef.current} reset={resetPath} />
          </div>
        </div>

        {/* Sensor + Button Panel */}
        <div className="col-12 col-md-3 d-flex flex-column gap-3">
          <div className="border rounded p-3 shadow-sm">
            <SensorData value={value} />
          </div>

          <button
            aria-live="polite"
            className={`btn ${socketRef.current ? 'btn-danger' : 'btn-success'} w-100 d-flex justify-content-center align-items-center gap-2`}
            onClick={socketRef.current ? handleStop : handleStart}
            disabled={isLoading}
          >
            {isLoading && <span className="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>}
            {isLoading ? "Processing..." : socketRef.current ? "Stop Logging" : "Start Logging"}
          </button>
        </div>
      </div>
    </div>
  );
};

export default MapContainer;
