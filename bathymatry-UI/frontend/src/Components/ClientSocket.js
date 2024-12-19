
import { useState, useEffect } from "react";

function ClientSocket() {
  const [value, setValue] = useState("");
  const [socket, setSocket] = useState(null);

  useEffect(() => {
 
    const ws = new WebSocket("ws://localhost:5001");

    // Handle WebSocket open event
    ws.onopen = () => {
      console.log("Connected to WebSocket server");
      ws.send("Hello Server!"); // Send initial message
    };

    // Handle messages from the server
    ws.onmessage = (event) => {
      //console.log(`Message from server: ${event.data}`);
      setValue(event.data);
    };

    ws.onclose = () => {
      console.log("Disconnected from WebSocket server");
    };

    ws.onerror = (error) => {
      console.error("WebSocket error:", error);
    };

    setSocket(ws);

    // Cleanup function to close the WebSocket connection on component unmount
    return () => {
      ws.close();
      console.log("WebSocket connection closed");
    };
  }, []); // Empty dependency array ensures this runs only once when the component mounts

  return (
    <>
      <h1>Echo Sounder Value: {value}</h1>
    </>
  );
}

export default ClientSocket;




