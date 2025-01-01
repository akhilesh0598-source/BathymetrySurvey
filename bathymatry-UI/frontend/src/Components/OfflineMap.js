// import { MapContainer, Marker, Popup, TileLayer } from 'react-leaflet';
// import { useRef, useMemo, useCallback, useState } from 'react';




// function OfflineMap() {
//     return (
//         <>
//             <MapContainer center={[51.505, -0.09]} zoom={13} scrollWheelZoom={false} style={{ height: '100vh', width: '100wh' }}>
//                 <TileLayer
//                     attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
//                     url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
//                 />
//                 <Marker position={[51.505, -0.09]}>
//                     <Popup>
//                         A pretty CSS3 popup. <br /> Easily customizable.
//                     </Popup>
//                 </Marker>
//             </MapContainer>
//         </>
//     )
// }
// export default OfflineMap;


import React, { Component } from 'react'
import Leaflet from 'leaflet';
import { MapContainer, TileLayer, Marker, Popup } from 'react-leaflet'
import 'leaflet/dist/leaflet.css';

Leaflet.Icon.Default.imagePath =
    '../node_modules/leaflet'

delete Leaflet.Icon.Default.prototype._getIconUrl;

Leaflet.Icon.Default.mergeOptions({
    iconRetinaUrl: require('leaflet/dist/images/marker-icon-2x.png'),
    iconUrl: require('leaflet/dist/images/marker-icon.png'),
    shadowUrl: require('leaflet/dist/images/marker-shadow.png')
});

export default class OfflineMap extends Component {
    // state = {
    //     lat: 10.257017,
    //     lng: -29.077524,
    //     zoom: 13,
    // }
    state=this.props;

    render() {
        const position = [this.state.lat, this.state.lng]
        return (
            <MapContainer center={position} zoom={this.state.zoom} style={{ height: '400px' }}>
                <TileLayer
                    attribution='&amp;copy <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
                    url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
                />
                <Marker position={position}>
                    <Popup>
                        Son Konum
                    </Popup>
                </Marker>
            </MapContainer>
        )
    }
}