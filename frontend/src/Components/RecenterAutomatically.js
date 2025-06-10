import { useMap } from 'react-leaflet';
import { useEffect } from 'react';

const RecenterAutomatically = ({ lat, lng, zoom }) => {
  const map = useMap();

  useEffect(() => {
    map.flyTo([lat, lng], zoom, {
      animate: true,
      duration: 1.5, // seconds, adjust as needed
    });
  }, [lat, lng, zoom, map]);

  return null;
};

export default RecenterAutomatically;
