import random
import time

# Path to the mock serial device
gps_device_path = "/tmp/ttyWriteGPS"

def generate_lat_lon():
    # Latitude: 0-89 degrees, Longitude: 0-179 degrees
    lat_deg = random.randint(0, 89)
    lon_deg = random.randint(0, 179)

    # Latitude and Longitude decimal minutes
    lat_min = random.uniform(0, 60)
    lon_min = random.uniform(0, 60)

    # Format lat/lon in NMEA style (ddmm.mmmm / dddmm.mmmm)
    lat_str = f"{lat_deg:02d}{lat_min:07.4f}"
    lon_str = f"{lon_deg:03d}{lon_min:07.4f}"

    # Random direction
    lat_dir = random.choice(["N", "S"])
    lon_dir = random.choice(["E", "W"])

    return lat_str, lat_dir, lon_str, lon_dir

def generate_gpgga_sentence():
    lat, lat_dir, lon, lon_dir = generate_lat_lon()
    # Static fields for other GPGGA parts (UTC, fix quality, satellites, etc.)
    base = f"GPGGA,123519,{lat},{lat_dir},{lon},{lon_dir},1,08,0.9,545.4,M,46.9,M,,"
    checksum = calculate_checksum(base)
    return f"${base}*{checksum}"

def calculate_checksum(nmea_str):
    """Calculate NMEA checksum (XOR of all characters between $ and *)"""
    cksum = 0
    for c in nmea_str:
        cksum ^= ord(c)
    return f"{cksum:02X}"

def main():
    print(f"Sending mock GPS data to {gps_device_path}")
    while True:
        sentence = generate_gpgga_sentence()
        with open(gps_device_path, "w") as f:
            f.write(sentence + "\r\n")
        print("Sent:", sentence)
        time.sleep(1)

if __name__ == "__main__":
    main()
