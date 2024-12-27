
import { useState, useEffect } from "react";

function ClientSocket() 
{
  const [value, setValue] = useState({});
  const [socket, setSocket] = useState(null);

  const openWebSocket = () => {

    if (socket) return;
    const ws = new WebSocket("ws://localhost:5001");

    ws.onopen = () => {
      console.log("Connected to WebSocket server");
      ws.send("Hello Server!");
      setSocket(ws);
    };

    ws.onmessage = (event) => {
      try {
        const parsedData = JSON.parse(event.data);
        setValue(parsedData);
      } catch (error) {
        console.error("Parsing Error:", error);
      }
    };

    ws.onclose = () => {
      console.log("Disconnected from WebSocket server");
      setValue({});
    };

    ws.onerror = (error) => {
      console.error("WebSocket error:", error);
    };

  };

  const handleStop = () => {
    if (socket) {
      socket.close();
      setSocket(null);
      console.log("WebSocket is stopped!");
    }
    setValue({});

  };

  const handleStart = () => {
    
      openWebSocket();
      console.log("WebSocket is starting...");
    
  };

  useEffect(() => {

    return () => {
      if (socket) {
        socket.close();
      }
    };
  }, [socket]);

  return (
    <>
      <h1>Echo Sounder Values: </h1>
      <h2>Distance: {value.distance || "N/A"}</h2>
      <h2>Confidence: {value.confidence || "N/A"}</h2>
      <h2>GPS UTCTime: {value.gpsDateTime || "N/A"}</h2>
      <h2>GPS Latitude: {value.gpsLatitude || "N/A"}</h2>
      <h2>GPS Longitude: {value.gpsLongitude || "N/A"}</h2>

      <button onClick={socket ? handleStop : handleStart}>
        {socket ? "Stop Reading" : "Start Reading"}
      </button>
    </>
  );
}

export default ClientSocket;




