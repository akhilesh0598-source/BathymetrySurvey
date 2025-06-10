import React, { useEffect, useRef, useState } from 'react';
import Leaflet from 'leaflet';
import { MapContainer, TileLayer, Marker, Popup, Polyline, useMap } from 'react-leaflet';
import 'leaflet/dist/leaflet.css';

// Component to recenter the map smoothly (debounced)
const RecenterAutomatically = ({ lat, lng, zoom }) => {
  const map = useMap();
  const target = useRef({ lat, lng, zoom });

  useEffect(() => {
    target.current = { lat, lng, zoom };
    const timeout = setTimeout(() => {
      map.setView([target.current.lat, target.current.lng], target.current.zoom);
    }, 300); // 300ms debounce
    return () => clearTimeout(timeout);
  }, [lat, lng, zoom, map]);

  return null;
};

Leaflet.Icon.Default.imagePath = '../node_modules/leaflet';

delete Leaflet.Icon.Default.prototype._getIconUrl;

Leaflet.Icon.Default.mergeOptions({
  iconRetinaUrl: require('leaflet/dist/images/marker-icon-2x.png'),
  iconUrl: require('leaflet/dist/images/marker-icon.png'),
  shadowUrl: require('leaflet/dist/images/marker-shadow.png'),
});

const OfflineMap = ({ gpsData, isTracking, reset }) => {
  const [path, setPath] = useState([]);
  const isFirstRun = useRef(true);

  useEffect(() => {
    if (reset) {
      setPath([]);
      isFirstRun.current = true;
    }
  }, [reset]);

  useEffect(() => {
    if (gpsData.latitude && gpsData.longitude) {
      const newPoint = [gpsData.latitude, gpsData.longitude];

      if (isFirstRun.current) {
        isFirstRun.current = false;
        return;
      }

      if (isTracking) {
        setPath(prev => [...prev, newPoint]);
      }
    }
  }, [gpsData, isTracking]);

  const position = [gpsData.latitude, gpsData.longitude];
  const zoom = gpsData.zoomVal || 13;

  return (
    <div className="card h-100">
      <div className="card-body p-2">
        <MapContainer
          center={position}
          zoom={zoom}
          style={{ height: '100%', minHeight: '400px', width: '100%' }}
          minZoom={3}
          maxZoom={19}
          scrollWheelZoom={true}
          // Don't recreate map instance on every render:
          key="map"
        >
          <TileLayer
            attribution='&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
            url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
          />

          <Marker position={position}>
            <Popup>Current Position</Popup>
          </Marker>

          {path.length > 1 && <Polyline positions={path} color="blue" />}

          <RecenterAutomatically lat={gpsData.latitude} lng={gpsData.longitude} zoom={zoom} />
        </MapContainer>
      </div>
    </div>
  );
};

export default OfflineMap;
