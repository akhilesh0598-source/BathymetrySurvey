import time
from functools import reduce
from datetime import datetime, timezone

GPS_DEVICE_PATH = "/tmp/ttyWriteGPS"  # Update this to your virtual port
START_LAT = 23.2599
START_LON = 77.4126
LAT_STEP = 0.0001
LON_STEP = 0.0001
UPDATE_INTERVAL = 1  # seconds

def decimal_to_nmea(coord, is_lat):
    degrees = int(abs(coord))
    minutes = (abs(coord) - degrees) * 60
    if is_lat:
        direction = 'N' if coord >= 0 else 'S'
        fmt_str = f"{degrees:02d}{minutes:07.4f}"
    else:
        direction = 'E' if coord >= 0 else 'W'
        fmt_str = f"{degrees:03d}{minutes:07.4f}"
    return fmt_str, direction

def calculate_checksum(nmea_sentence_data):
    checksum = reduce(lambda a, b: a ^ ord(b), nmea_sentence_data, 0)
    return f"{checksum:02X}"

def generate_gpgga(lat, lon):
    utc_time = datetime.now(timezone.utc).strftime("%H%M%S.000")
    lat_str, lat_dir = decimal_to_nmea(lat, True)
    lon_str, lon_dir = decimal_to_nmea(lon, False)
    base_sentence = f"GPGGA,{utc_time},{lat_str},{lat_dir},{lon_str},{lon_dir},1,08,0.9,100.0,M,0.0,M,,"
    return f"${base_sentence}*{calculate_checksum(base_sentence)}"

def main():
    lat, lon = START_LAT, START_LON

    try:
        with open(GPS_DEVICE_PATH, "w") as f:
            while True:
                nmea = generate_gpgga(lat, lon)
                #print(f"Sending: {nmea}")
                f.write(nmea + "\r\n")
                f.flush()

                # Increment position
                lat += LAT_STEP
                lon += LON_STEP

                time.sleep(UPDATE_INTERVAL)

    except FileNotFoundError:
        print(f"Error: Device path '{GPS_DEVICE_PATH}' not found. Create it using socat.")
    except Exception as e:
        print(f"Unexpected error: {e}")

if __name__ == "__main__":
    main()
