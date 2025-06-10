import React from 'react';

const SensorsData = ({ value }) => {
  return (
    <div className="card h-100">
      <div className="card-body">
        <h5 className="card-title text-center mb-4">Echo Sounder and GPS Data</h5>
        <div className="d-flex flex-column gap-2">
          <p className="mb-1"><strong>Distance:</strong> <span>{value.distance} m</span></p>
          <p className="mb-1"><strong>Confidence:</strong> <span>{value.confidence} %</span></p>
          <p className="mb-1"><strong>Current Time:</strong> <span>{value.gpsDateTime}</span></p>
          <p className="mb-1"><strong>Latitude:</strong> <span>{value.latitude}</span></p>
          <p className="mb-1"><strong>Longitude:</strong> <span>{value.longitude}</span></p>
        </div>
      </div>
    </div>
  );
};

export default SensorsData;
