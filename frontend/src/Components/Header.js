import React, { useState, useRef } from 'react';
import { FaDownload, FaPlay, FaStop, FaBars } from 'react-icons/fa';
import { getFromUrl, downloadFile } from '../api/FetchData';
import { toast } from 'react-toastify';

import { Link } from 'react-router-dom';
import { Nav, Button } from 'react-bootstrap';

const Header = () => {
  const [isLogging, setIsLogging] = useState(false);
  const [menuOpen, setMenuOpen] = useState(false);
  const alertShownRef = useRef(false);
  const [loading, setLoading] = useState(false);
  const [downloadLoading, setDownloadLoading] = useState(false);

  const toggleMenu = () => setMenuOpen(!menuOpen);

  const handleLoggingToggle = async () => {
    if (alertShownRef.current || loading) return;
    alertShownRef.current = true;
    setLoading(true);

    if (!isLogging) {
      try {
        await getFromUrl('/startLogging');
        setIsLogging(true);
        toast.success("Logging started!");
      } catch (error) {
        toast.error("Failed to start logging.");
        setIsLogging(false);
      }
    } else {
      try {
        await getFromUrl('/stopLogging');
        setIsLogging(false);
        toast.success("Logging stopped!");
      } catch (error) {
        toast.error("Failed to stop logging.");
      }
    }

    setLoading(false);
    setTimeout(() => {
      alertShownRef.current = false;
    }, 1000);
  };

  const downloadData = async () => {
    setDownloadLoading(true);
    try {
      const url = '/download';
      await downloadFile(url, 'device_log');
      console.log('Download data triggered...');
      toast.success("Download started successfully");
    } catch (error) {
      console.error('Download failed:', error);
      toast.error("Failed to start download");
    } finally {
      setDownloadLoading(false);
    }
  };

  return (
    <header className="bg-info shadow-sm sticky-top">
      <div className="container py-3">
        {/* Top Row: Title and FaBars */}
        <div className="d-flex justify-content-between align-items-center">
          <div className="text-white fw-bold fs-4">
            📡 Bathymetry Tracker
          </div>

          <button
            className="btn btn-outline-light d-md-none"
            onClick={toggleMenu}
            aria-label="Toggle navigation"
          >
            <FaBars />
          </button>

          {/* Desktop Menu (Right Side) */}
          <div className="d-none d-md-flex align-items-center gap-3">
            <button
              onClick={handleLoggingToggle}
              className={`btn ${isLogging ? 'btn-danger' : 'btn-success'} d-flex align-items-center justify-content-center`}
              title={isLogging ? 'Stop Logging' : 'Start Logging'}
              disabled={loading}
            >
              <span className="me-2">
                {loading ? (isLogging ? 'Stopping...' : 'Starting...') : (isLogging ? 'Stop Logging' : 'Start Logging')}
              </span>
              {loading ? (
                <span className="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>
              ) : (
                isLogging ? <FaStop /> : <FaPlay />
              )}
            </button>

            <button
              onClick={downloadData}
              className="btn btn-primary d-flex align-items-center justify-content-center"
              title="Download Data"
              disabled={downloadLoading} // optional: disable while loading
            >
              {downloadLoading ? (
                <>
                  <span className="spinner-border spinner-border-sm me-2" role="status" />
                  Downloading...
                </>
              ) : (
                <>
                  <span className="me-2">Download Data</span>
                  <FaDownload />
                </>
              )}
            </button>

            <Nav.Item>
              <Nav.Link

                href="/about"
                className="btn btn-outline-success fw-semibold px-1"
              >
                About Me
              </Nav.Link>
            </Nav.Item>

            <Nav.Item>
              <Nav.Link

                href="/Akhilesh_Chaurasiya_Resume.pdf"
                className="btn btn-outline-success fw-semibold px-2"
              >
                📄View Resume
              </Nav.Link>
            </Nav.Item>

          </div>
        </div>
        {menuOpen && (
          <div className="d-md-none mt-3 bg-light p-3 rounded d-flex flex-column gap-2">
            <button
              onClick={handleLoggingToggle}
              className={`btn ${isLogging ? 'btn-danger' : 'btn-success'} d-flex align-items-center justify-content-center`}
              title={isLogging ? 'Stop Logging' : 'Start Logging'}
              disabled={loading}
            >
              <span className="me-2">
                {loading ? (isLogging ? 'Stopping...' : 'Starting...') : (isLogging ? 'Stop Logging' : 'Start Logging')}
              </span>
              {loading ? (
                <span className="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>
              ) : (
                isLogging ? <FaStop /> : <FaPlay />
              )}
            </button>
            <button
              onClick={downloadData}
              className="btn btn-primary d-flex align-items-center justify-content-center"
              title="Download Data"
              disabled={downloadLoading} // optional: disable while loading
            >
              {downloadLoading ? (
                <>
                  <span className="spinner-border spinner-border-sm me-2" role="status" />
                  Downloading...
                </>
              ) : (
                <>
                  <span className="me-2">Download Data</span>
                  <FaDownload />
                </>
              )}
            </button>

            <Nav.Item>
              <Nav.Link

                href="/about"
                className="btn btn-outline-success fw-semibold px-1"
              >
                About Me
              </Nav.Link>
            </Nav.Item>

            <Nav.Item>
              <Nav.Link

                href="/Akhilesh_Chaurasiya_Resume.pdf"
                className="btn btn-outline-success fw-semibold px-2"
              >
                📄View Resume
              </Nav.Link>
            </Nav.Item>
          </div>
        )}
      </div>
    </header>
  );
};

export default Header;
