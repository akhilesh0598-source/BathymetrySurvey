import serial

SERIAL_PORT = "/tmp/ttyReadPing"
BAUDRATE = 115200

ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)

print("Listening for requests from main program...")

while True:
    data = ser.read(16)  # read up to 16 bytes (adjust if needed)
    if data:
        print(f"Received {len(data)} bytes: {data.hex()}")
