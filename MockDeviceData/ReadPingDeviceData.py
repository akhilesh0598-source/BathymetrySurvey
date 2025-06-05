import serial
import struct
import time
import sys

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

# Format string for struct.unpack:
# < : Little-endian
# I : unsigned int (4 bytes)
# H : unsigned short (2 bytes)
# B : unsigned char (1 byte)
# The Python mock device packs gain_setting as 'H' (uint16_t), so we must unpack it as 'H' here too.
DISTANCE_PAYLOAD_FORMAT = '<IHHIIIH' # distance, confidence, transmit_duration, ping_number, scan_start, scan_length, gain_setting

# --- Helper Functions ---

def calculate_checksum(data_bytes):
    """Calculates the 16-bit checksum for Ping Protocol messages."""
    checksum = 0
    for byte_val in data_bytes:
        checksum += byte_val
    return checksum & 0xFFFF # Ensure it's a 16-bit value

def parse_ping_message(buffer):
    """
    Parses the receive buffer for a complete Ping Protocol message.
    Returns (message_dict, bytes_consumed) if a complete message is found,
    otherwise (None, 0).
    """
    if len(buffer) < 8: # Minimum header size (start1, start2, payload_length, message_id, src_id, dst_id)
        return None, 0

    # Look for start bytes
    start_index = -1
    for i in range(len(buffer) - 1):
        if buffer[i] == START_BYTE_1 and buffer[i+1] == START_BYTE_2:
            start_index = i
            break

    if start_index == -1:
        # No start bytes found, consume all bytes
        return None, len(buffer)

    # If start bytes are not at the beginning, discard leading junk
    if start_index > 0:
        buffer = buffer[start_index:]
        # print(f"Discarded {start_index} junk bytes. New buffer len: {len(buffer)}")
        # If after discarding, buffer is too small for header, return
        if len(buffer) < 8:
            return None, start_index # Indicate how many bytes were removed

    # Now, buffer should start with START_BYTE_1 and START_BYTE_2
    # Extract header fields
    try:
        # Header: start1(B), start2(B), payload_length(H), message_id(H), src_device_id(B), dst_device_id(B)
        header_bytes = buffer[0:8]
        start1, start2, payload_length, message_id, src_device_id, dst_device_id = \
            struct.unpack('<BBHHBB', bytes(header_bytes))
    except struct.error as e:
        # Not enough bytes for header, or malformed. Consume 1 byte to try again.
        # print(f"Struct unpack error for header: {e}. Buffer len: {len(buffer)}")
        return None, 1 # Consume one byte to slide window

    expected_total_len = 8 + payload_length + 2 # Header (8) + Payload + Checksum (2)

    if len(buffer) < expected_total_len:
        # Not enough bytes for the full message yet
        return None, 0

    full_message_bytes = buffer[0:expected_total_len]

    # Extract checksum from the end of the full message
    received_checksum = struct.unpack('<H', bytes(full_message_bytes[-2:]))[0]

    # Calculate checksum on all bytes *before* the checksum field
    checksum_data = full_message_bytes[:-2]
    calculated_checksum = calculate_checksum(checksum_data)

    if received_checksum != calculated_checksum:
        print(f"Checksum Mismatch! Received: 0x{received_checksum:04x}, Calculated: 0x{calculated_checksum:04x}", file=sys.stderr)
        print(f"  Raw Bytes: {bytes(full_message_bytes).hex()}", file=sys.stderr)
        return None, 1 # Checksum mismatch, discard first byte and try again

    # Checksum matches, process the message
    message_data = {
        'start1': start1,
        'start2': start2,
        'payload_length': payload_length,
        'message_id': message_id,
        'src_device_id': src_device_id,
        'dst_device_id': dst_device_id,
        'checksum': received_checksum,
        'raw_bytes': full_message_bytes
    }

    if message_id == MESSAGE_ID_DISTANCE:
        if payload_length == struct.calcsize(DISTANCE_PAYLOAD_FORMAT):
            try:
                payload_bytes = full_message_bytes[8:-2] # Payload is between header and checksum
                distance_mm, confidence_percent, transmit_duration_us, \
                ping_number, scan_start_mm, scan_length_mm, gain_setting = \
                    struct.unpack(DISTANCE_PAYLOAD_FORMAT, bytes(payload_bytes))

                message_data['type'] = 'distance'
                message_data['distance_mm'] = distance_mm
                message_data['confidence_percent'] = confidence_percent
                message_data['transmit_duration_us'] = transmit_duration_us
                message_data['ping_number'] = ping_number
                message_data['scan_start_mm'] = scan_start_mm
                message_data['scan_length_mm'] = scan_length_mm
                message_data['gain_setting'] = gain_setting
            except struct.error as e:
                print(f"Error unpacking distance payload: {e}", file=sys.stderr)
                return None, 1 # Malformed payload, discard first byte
        else:
            print(f"Distance message payload length mismatch. Expected {struct.calcsize(DISTANCE_PAYLOAD_FORMAT)}, got {payload_length}", file=sys.stderr)
            return None, 1 # Mismatch, discard first byte
    else:
        message_data['type'] = 'unknown'
        message_data['payload'] = full_message_bytes[8:-2] # Store raw payload for unknown messages

    return message_data, expected_total_len

# --- Main Program ---

if __name__ == "__main__":
    # IMPORTANT: Replace with the OTHER PTY path from your socat output
    # If Python mock device uses /dev/pts/0, use /dev/pts/1 here, or vice-versa.
    SERIAL_PORT = '/tmp/ttyReadPing' # CHANGE THIS TO YOUR SOCAT PTY DEVICE

    print(f"Attempting to open serial port: {SERIAL_PORT} at {DEFAULT_BAUD_RATE} bps")

    try:
        ser = serial.Serial(
            SERIAL_PORT,
            DEFAULT_BAUD_RATE,
            timeout=0.1, # Small timeout for read operations
            write_timeout=0 # Not writing, but good practice
        )
        print("Serial port opened successfully.")

        receive_buffer = bytearray() # Use bytearray for efficient appending and slicing

        while True:
            try:
                # Read available bytes
                bytes_read = ser.read(ser.in_waiting or 1) # Read all available, or at least 1 if none
                if bytes_read:
                    receive_buffer.extend(bytes_read)

                # Attempt to parse messages from the buffer
                message, bytes_consumed = parse_ping_message(receive_buffer)

                if message:
                    if message['type'] == 'distance':
                        print(f"Received Ping Message (ID: {message['message_id']}):")
                        print(f"  Distance: {message['distance_mm']} mm")
                        print(f"  Confidence: {message['confidence_percent']} %")
                        print(f"  Ping Number: {message['ping_number']}")
                        print(f"  Raw Bytes: {bytes(message['raw_bytes']).hex()}")
                        print("-------------------------------------")
                    else:
                        print(f"Received Unknown Message (ID: {message['message_id']}). Payload Length: {message['payload_length']}")
                        print(f"  Raw Bytes: {bytes(message['raw_bytes']).hex()}")
                        print("-------------------------------------")
                    # Remove consumed bytes from the buffer
                    receive_buffer = receive_buffer[bytes_consumed:]
                elif bytes_consumed > 0:
                    # If parse_ping_message returned bytes_consumed > 0 but no message,
                    # it means junk bytes were discarded or a malformed message part was skipped.
                    receive_buffer = receive_buffer[bytes_consumed:]

                # Prevent buffer from growing indefinitely if no valid messages are found
                # This also helps clear out partial, invalid messages that might accumulate
                if len(receive_buffer) > 256: # Max expected message size + some buffer
                    print("Buffer growing too large, clearing...", file=sys.stderr)
                    receive_buffer = bytearray() # Clear buffer

            except serial.SerialException as e:
                print(f"Serial read error: {e}", file=sys.stderr)
                break
            except Exception as e:
                print(f"An unexpected error occurred: {e}", file=sys.stderr)
                break

            time.sleep(0.01) # Small delay to prevent busy-waiting

    except serial.SerialException as e:
        print(f"Failed to open serial port {SERIAL_PORT}: {e}", file=sys.stderr)
        print("Please ensure the socat command is running and the device path is correct.", file=sys.stderr)
    except KeyboardInterrupt:
        print("\nExiting Python reader.", file=sys.stderr)
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Serial port closed.", file=sys.stderr)

