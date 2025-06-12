import React, { useState, useEffect, useRef } from 'react';
import OfflineMap from './OfflineMap';
import SensorData from './SensorData';
import config from '../config';
import { toast } from 'react-toastify';
import {getWithUrl} from '../api/FetchData'

const DEFAULT_DATA = {
  distance: "",
  confidence: "",
  latitude: "23.406855",
  longitude: "77.694556",
  gpsDateTime: "00:00:00",
  zoomVal: 3,
};

const MapContainer = () => {
  const [value, setValue] = useState(DEFAULT_DATA);
  const [isLoading, setIsLoading] = useState(false);
  const [resetPath, setResetPath] = useState(false);
  const [isTracking, setIsTracking] = useState(false);
  const pollingRef = useRef(null);

const fetchData = async () => {
  try {
    const response = await getWithUrl('/data/devices');

    // Add zoomVal to the parsed data object
    response.zoomVal = 12;
    // Merge new data with previous state
    setValue(prev => ({ ...prev, ...response }));

  } catch (error) {
    console.error('Fetch error:', error);
    toast.error("Failed to fetch sensor data");
  }
};


  const handleStart = () => {
    setResetPath(true);
    setIsTracking(true);
    setIsLoading(true);
    fetchData(); // Fetch immediately

    pollingRef.current = setInterval(fetchData, 500); // Fetch every second

    setTimeout(() => {
      setIsLoading(false);
      toast.success("Started tracking");
    }, 500);
  };

  const handleStop = () => {
    if (pollingRef.current) {
      clearInterval(pollingRef.current);
      pollingRef.current = null;
    }

    setIsTracking(false);
    setValue(DEFAULT_DATA);
    setResetPath(false);
    toast.info("Tracking stopped");
  };

  useEffect(() => {
    return () => {
      // Cleanup on unmount
      if (pollingRef.current) {
        clearInterval(pollingRef.current);
        pollingRef.current = null;
      }
    };
  }, []);

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
            <OfflineMap gpsData={value} isTracking={isTracking} reset={resetPath} />
          </div>
        </div>

        {/* Sensor + Button Panel */}
        <div className="col-12 col-md-3 d-flex flex-column gap-3">
          <div className="border rounded p-3 shadow-sm">
            <SensorData value={value} />
          </div>

          <button
            className={`btn ${isTracking ? 'btn-danger' : 'btn-success'} w-100 d-flex justify-content-center align-items-center gap-2`}
            onClick={isTracking ? handleStop : handleStart}
            disabled={isLoading}
          >
            {isLoading && <span className="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>}
            {isLoading ? "Processing..." : isTracking ? "Stop Logging" : "Start Logging"}
          </button>
        </div>
      </div>
    </div>
  );
};

export default MapContainer;
