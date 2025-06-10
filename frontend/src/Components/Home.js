import React from 'react';
import Header from './Header';
import Footer from './Footer';
import MapContainer from './MapContainer';
import { ToastContainer } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';

function Home() {

  return (
    <div className="container mt-0">
      <Header />
      <ToastContainer position="top-right" autoClose={3000} />
      <MapContainer />
      <Footer />
    </div>
  );
}

export default Home;
