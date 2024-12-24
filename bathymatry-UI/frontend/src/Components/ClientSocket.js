
import { useState } from "react";

function ClientSocket() {
  const [value, setValue] = useState({});
  const [socket, setSocket] = useState(null);

  const openWebSocket=() => {
 
    const ws = new WebSocket("ws://localhost:5001");
    setSocket(ws);
    // Handle WebSocket open event
    ws.onopen = () => {
      console.log("Connected to WebSocket server");
      ws.send("Hello Server!"); // Send initial message
    };

    // Handle messages from the server
    ws.onmessage = (event) => {
      //console.log(`Message from server: ${event.data}`);
      const data=event.data;
      try{
      const parsedData=JSON.parse(data);
      setValue(parsedData);  
      }
      catch
      {
        console.log("Parsing Error!!");
      }
      
    };

    ws.onclose = () => {
      console.log("Disconnected from WebSocket server");
      if(socket)
        setTimeout(openWebSocket,1000);
    };

    ws.onerror = (error) => {
      console.error("WebSocket error:", error);
    };

  };

const handleStop=()=>{
  if(socket)
  {
    socket.close();
    setSocket(null);
    setValue('')
    console.log("Websocket is stopped!!");
  }
};

const handleStart=()=>{
  if(!socket || socket.readyState===WebSocket.CLOSED)
  {
    openWebSocket();
    console.log("Websocket is opened!!");
  }
}

  return (
    <>
      <h1>Echo Sounder Values: </h1>
      <h2>Distance: {value.distance}</h2>
      <h2>Confidence: {value.confidence}</h2>
      <button onClick={socket==null? handleStart:handleStop}>{socket==null?"Start Reading":"Stop Reading"}</button>
    </>
  );
}

export default ClientSocket;




