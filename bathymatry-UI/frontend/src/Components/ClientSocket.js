//import { useEffect, useState } from 'react';


function ClientSocket()
{
    //const [socket,setSocket]=useState(null);
    const socket = new WebSocket("ws://localhost:5001");
    // useEffect(()=>{
    //     
    //     setSocket(socket);
    //     socket.onopen = () => {
    //       console.log("Connected to WebSocket server");
    //       socket.send("Hello Server!");
    //     };
        
    //     socket.onmessage = (event) => {
    //       console.log(`Message from server: ${event.data}`);
    //     };
        
    //     socket.onclose = () => {
    //       console.log("Disconnected");
    //     };
        
    // },[]);
    socket.addEventListener("open",()=>{
        console.log("opended");
        socket.send("Hello ws");
    })
    socket.addEventListener("message", (event) => {
        console.log("Message from server ", event.data);
      });

    return (
        <>
            <h1>Hello World</h1>
        </>
    )
}
export default ClientSocket;