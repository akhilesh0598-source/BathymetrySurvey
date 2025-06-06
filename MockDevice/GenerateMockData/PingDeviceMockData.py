import serial
import struct
import time
import random

# --- Ping Protocol Constants ---
START_BYTE_1 = 0x42  # 'B'
START_BYTE_2 = 0x52  # 'R'
MESSAGE_ID_DISTANCE = 1212  # Ping1D distance message ID
DEFAULT_BAUD_RATE = 115200

# --- Ping Protocol Message Structure (Distance Message - ID 1212) ---
# This structure is based on the Ping Protocol documentation for message ID 1212
# 'distance' (u32), 'confidence' (u16), 'transmit_duration' (u16),
# 'ping_number' (u32), 'scan_start' (u32), 'scan_length' (u32),
# 'gain_setting' (u8) - Note: The documentation shows u32 for gain_setting,
# but often it's represented as u8 in examples. We'll use u8 for simplicity.

# Format string for struct.pack:
# < : Little-endian
# B : unsigned char (1 byte)
# H : unsigned short (2 bytes)
# I : unsigned int (4 bytes)
DISTANCE_MESSAGE_FORMAT = '<IHHIIIH' # distance, confidence, transmit_duration, ping_number, scan_start, scan_length, gain_setting (u8 padded to H for struct)

# --- Helper Functions ---

def calculate_checksum(data_bytes):
    """Calculates the 16-bit checksum for Ping Protocol messages."""
    checksum = 0
    for byte_val in data_bytes:
        checksum += byte_val
    return checksum & 0xFFFF # Ensure it's a 16-bit value

def create_distance_message(
    distance_mm,
    confidence_percent,
    transmit_duration_us,
    ping_number,
    scan_start_mm,
    scan_length_mm,
    gain_setting
):
    """
    Creates a complete Ping Protocol distance message (ID 1212).
    All values are in little-endian format.
    """
    # Payload for distance message (ID 1212)
    # The gain_setting is u8, but struct.pack needs a format specifier that matches the size.
    # We'll pack it as a short (H) and ensure the value fits in u8.
    payload_data = struct.pack(
        DISTANCE_MESSAGE_FORMAT,
        distance_mm,
        confidence_percent,
        transmit_duration_us,
        ping_number,
        scan_start_mm,
        scan_length_mm,
        gain_setting # This will be packed as a short due to H, but the value will be correct
    )

    payload_length = len(payload_data)
    message_id = MESSAGE_ID_DISTANCE
    src_device_id = 0 # Not currently implemented, typically 0
    dst_device_id = 0 # Not currently implemented, typically 0

    # Header bytes (excluding checksum for calculation)
    header_prefix = struct.pack(
        '<BBHHBB',
        START_BYTE_1,
        START_BYTE_2,
        payload_length,
        message_id,
        src_device_id,
        dst_device_id
    )

    # Combine header prefix and payload for checksum calculation
    checksum_data = header_prefix + payload_data
    checksum = calculate_checksum(checksum_data)

    # Final message bytes
    full_message = header_prefix + payload_data + struct.pack('<H', checksum)

    return full_message

# --- Main Program ---

if __name__ == "__main__":
    # IMPORTANT: Replace with one of the PTY paths from your socat output
    # For example, if socat output was /dev/pts/0 and /dev/pts/1, use one here.
    # The other will be used by the C++ program.
    SERIAL_PORT = '/tmp/ttyWritePing' # CHANGE THIS TO YOUR SOCAT PTY DEVICE

    print(f"Attempting to open serial port: {SERIAL_PORT} at {DEFAULT_BAUD_RATE} bps")

    try:
        ser = serial.Serial(
            SERIAL_PORT,
            DEFAULT_BAUD_RATE,
            timeout=0.1, # Small timeout for non-blocking read if needed
            write_timeout=0.1 # Small timeout for non-blocking write
        )
        print("Serial port opened successfully.")

        ping_count = 0
        while True:
            # Generate mock Ping1D data
            distance = random.randint(300, 50000)  # 0.3m to 50m in mm
            confidence = random.randint(50, 100)   # 50% to 100%
            transmit_duration = random.randint(100, 500) # microseconds
            ping_number = ping_count
            scan_start = 300 # mm
            scan_length = 50000 # mm
            gain_setting = random.randint(0, 6) # 0-6 for gain settings

            message = create_distance_message(
                distance,
                confidence,
                transmit_duration,
                ping_number,
                scan_start,
                scan_length,
                gain_setting
            )

            try:
                ser.write(message)
                # print(f"Sent: {message.hex()} (Distance: {distance} mm, Confidence: {confidence}%)")
                print(f"Sent Ping {ping_number}: Distance {distance} mm, Confidence {confidence}%")
                ping_count += 1
            except serial.SerialTimeoutException:
                print("Write timeout occurred, could not send message.")
            except Exception as e:
                print(f"Error writing to serial port: {e}")
                break

            time.sleep(0.1) # Send a message every 100ms

    except serial.SerialException as e:
        print(f"Failed to open serial port {SERIAL_PORT}: {e}")
        print("Please ensure the socat command is running and the device path is correct.")
    except KeyboardInterrupt:
        print("\nExiting mock device generator.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Serial port closed.")

