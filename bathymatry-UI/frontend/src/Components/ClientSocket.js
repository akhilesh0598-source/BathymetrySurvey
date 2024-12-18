import { useState, useEffect } from "react";
import { io } from "socket.io-client"; // Import the Socket.IO client library

function ClientSocket() {
  const [value, setValue] = useState(""); // Store server messages
  const [socket, setSocket] = useState(null); // Socket instance

  useEffect(() => {
    // Connect to the Socket.IO server
    const socketInstance = io("ws://localhost:5001", {
        transports: ["websocket"], // Force WebSocket transport
      });
    setSocket(socketInstance);

    // Handle connection event
    socketInstance.on("connect", () => {
      console.log("Connected to WebSocket server");
      socketInstance.send("Hello Server!"); // Send a message
    });

    // Handle incoming messages
    socketInstance.on("message", (data) => {
      console.log("Message from server:", data);
      setValue(data); // Update the state with server data
    });

    // Handle disconnection
    socketInstance.on("disconnect", () => {
      console.log("Disconnected from server");
    });

    // Cleanup on component unmount
    return () => {
      socketInstance.disconnect();
      console.log("Socket disconnected");
    };
  }, []);

  return (
    <>
      <h1>Echo Sounder Value: {value}</h1>
    </>
  );
}

export default ClientSocket;




// import { useState, useEffect } from "react";

// function ClientSocket() {
//   const [value, setValue] = useState(""); // To store messages from the server
//   const [socket, setSocket] = useState(null); // WebSocket instance

//   useEffect(() => {
//     // Create a new WebSocket connection
//     const ws = new WebSocket("ws://localhost:5001");

//     // Handle WebSocket open event
//     ws.onopen = () => {
//       console.log("Connected to WebSocket server");
//       ws.send("Hello Server!"); // Send initial message
//     };

//     // Handle messages from the server
//     ws.onmessage = (event) => {
//       console.log(`Message from server: ${event.data}`);
//       setValue(event.data); // Update state with server data
//     };

//     // Handle WebSocket close event
//     ws.onclose = () => {
//       console.log("Disconnected from WebSocket server");
//     };

//     // Handle WebSocket errors
//     ws.onerror = (error) => {
//       console.error("WebSocket error:", error);
//     };

//     // Set WebSocket instance in state
//     setSocket(ws);

//     // Cleanup function to close the WebSocket connection on component unmount
//     return () => {
//       ws.close();
//       console.log("WebSocket connection closed");
//     };
//   }, []); // Empty dependency array ensures this runs only once when the component mounts

//   return (
//     <>
//       <h1>Echo Sounder Value: {value}</h1>
//     </>
//   );
// }

// export default ClientSocket;





// import {  useState } from 'react';


// function ClientSocket()
// {
//     const socket = new WebSocket("ws://localhost:5001");
   
//     const [value,setValue]=useState('');
//     socket.addEventListener("open",()=>{
//         console.log("opended");
//         socket.send("Hello ws");
//     })
//     socket.addEventListener("message", (event) => {
//         console.log("Message from server ", event.data);
//         setValue(event.data)
//       });

//     return (
//         <>
//             <h1>Echo Sounder Value: {value}</h1>
//         </>
//     )
// }
// export default ClientSocket;