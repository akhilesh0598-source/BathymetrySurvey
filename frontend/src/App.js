import React, { useState } from 'react';
import Header from './Components/Header';
import Footer from './Components/Footer';
import MapContainer from './Components/MapContainer';
import { ToastContainer } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';
import About from './Components/About';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import Home from './Components/Home';

function App() {

  return (
    <Router>
      <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/about" element={<About />} />
          {/* Add more routes as needed */}
      </Routes>
    </Router>
  );
}

export default App;
