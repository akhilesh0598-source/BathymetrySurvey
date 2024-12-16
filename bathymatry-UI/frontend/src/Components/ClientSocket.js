import { useEffect, useState } from 'react';
import io from 'socket.io-client';


function ClientSocket()
{
    const [socket,setSocket]=useState(null);
    useEffect(()=>{
        const socketInstance=io('ws://localhost:5001',{
            transports:['websocket'],
            withCredentials:true
        });
        setSocket(socketInstance);
    
        socketInstance.on('connect',()=>{
            console.log("Connected to Server");
        });
    
        socketInstance.on('message',(data)=>{
    
            console.log(`received message : ${data}`);
        })
        return ()=>{
            if(socketInstance)
            {
                socketInstance.disconnect();
            }
        }
    },[]);

    return (
        <>
            <h1>Hello World</h1>
        </>
    )
}
export default ClientSocket;