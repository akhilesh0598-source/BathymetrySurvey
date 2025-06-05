import { useState, useEffect } from "react";
import OfflineMap from "./OfflineMap";

function ClientSocket() 
{
  //const [serverStatus,setServerStatus]=useState('Not Connected!');
  const [value, setValue] = useState({distance:0,confidence:0,gpsLatitude:23.406855,gpsLongitude:77.694556,gpsDateTime:'00:00:00',zoomVal:3});
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
        parsedData['zoomVal']=14;
        //console.log(parsedData);
        console.log(parsedData.zoomVal);
        setValue(parsedData);
      } 
      catch (error) 
      {
        console.error("Parsing Error:", error);
      }
    };

    ws.onclose = () => {
      console.log("Disconnected from WebSocket server");
      setSocket(null);
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
    setValue({distance:0,confidence:0,gpsLatitude:23.406855,gpsLongitude:77.694556,gpsDateTime:'00:00:00',zoomVal:3});
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
      <h1>Connected status:  {!socket? 'Not Connected':'Connected'}</h1>
      <h1>Echo Sounder and GPS Values: </h1>
      <h2>Distance: {value.distance}</h2>
      <h2>Confidence: {value.confidence}</h2>
      <h2>GPS UTCTime: {value.gpsDateTime }</h2>
      <h2>GPS Latitude: {value.gpsLatitude }</h2>
      <h2>GPS Longitude: {value.gpsLongitude }</h2>

      <button onClick={socket ? handleStop : handleStart}>
        {socket ? "Stop Reading" : "Start Reading"}
      </button>

      <OfflineMap gpsData={value}/>
    </>
  );
}

export default ClientSocket;




