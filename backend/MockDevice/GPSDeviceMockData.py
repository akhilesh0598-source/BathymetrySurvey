import time
import math

# Path to the mock serial device
gps_device_path = "/tmp/ttyWriteGPS"

# Starting GPS coordinate (example: somewhere in Bangalore)
start_lat = 12.9716  # degrees
start_lon = 77.5946  # degrees

# Speed of movement in degrees per update
lat_step = 0.0001
lon_step = 0.0001

def decimal_to_nmea(coord, is_latitude):
    degrees = int(coord)
    minutes = (coord - degrees) * 60
    if is_latitude:
        direction = 'N' if coord >= 0 else 'S'
        return f"{abs(degrees):02d}{abs(minutes):07.4f}", direction
    else:
        direction = 'E' if coord >= 0 else 'W'
        return f"{abs(degrees):03d}{abs(minutes):07.4f}", direction

def generate_gpgga_sentence(lat, lon):
    lat_str, lat_dir = decimal_to_nmea(lat, is_latitude=True)
    lon_str, lon_dir = decimal_to_nmea(lon, is_latitude=False)

    # Static fields: UTC time, fix quality, satellites, etc.
    base = f"GPGGA,123519,{lat_str},{lat_dir},{lon_str},{lon_dir},1,08,0.9,545.4,M,46.9,M,,"
    checksum = calculate_checksum(base)
    return f"${base}*{checksum}"

def calculate_checksum(nmea_str):
    """Calculate NMEA checksum (XOR of all characters between $ and *)"""
    cksum = 0
    for c in nmea_str:
        cksum ^= ord(c)
    return f"{cksum:02X}"

def main():
    print(f"Sending simulated GPS data to {gps_device_path}")

    lat = start_lat
    lon = start_lon

    while True:
        sentence = generate_gpgga_sentence(lat, lon)
        try:
            with open(gps_device_path, "w") as f:
                f.write(sentence + "\r\n")
            print("Sent:", sentence)
        except IOError as e:
            print(f"Error writing to {gps_device_path}: {e}")

        # Move slightly
        lat += lat_step
        lon += lon_step
        time.sleep(1)

if __name__ == "__main__":
    main()
