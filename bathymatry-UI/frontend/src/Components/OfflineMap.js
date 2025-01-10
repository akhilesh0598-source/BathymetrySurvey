import React, { Component,useEffect } from 'react'
import Leaflet from 'leaflet';
import { MapContainer, TileLayer, Marker, Popup,useMap } from 'react-leaflet'
import 'leaflet/dist/leaflet.css';

const RecenterAutomatically = ({lat,lng}) => {
    const map = useMap();
     useEffect(() => {
       map.setView([lat, lng]);
     }, [lat, lng]);
     return null;
   }

Leaflet.Icon.Default.imagePath = '../node_modules/leaflet'

delete Leaflet.Icon.Default.prototype._getIconUrl;

Leaflet.Icon.Default.mergeOptions({
    iconRetinaUrl: require('leaflet/dist/images/marker-icon-2x.png'),
    iconUrl: require('leaflet/dist/images/marker-icon.png'),
    shadowUrl: require('leaflet/dist/images/marker-shadow.png')
});

export default class OfflineMap extends Component {

    render() {
        const position = [this.props.gpsData.gpsLatitude, this.props.gpsData.gpsLongitude];
        const zoomVal=this.props.gpsData.zoomVal;
        console.log(zoomVal);
        return (
            <MapContainer  center={position} zoom={zoomVal} style={{ height: '400px' }} minZoom={3} maxZoom={19} scrollWheelZoom={true}>
                <TileLayer
                    attribution='&amp;copy <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
                    url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
                />
                <Marker position={position} autoPan={true} autoPanOnFocus={true} bubblingMouseEvents={true}>
                    <Popup>
                        Son Konum
                    </Popup>
                </Marker>
                <RecenterAutomatically lat={this.props.gpsData.gpsLatitude} lng={this.props.gpsData.gpsLongitude} />
            </MapContainer>
        )
    }
}