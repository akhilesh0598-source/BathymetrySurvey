import serial
import time

ser = serial.Serial("/tmp/ttyReadPing", 115200, timeout=2)

# Send distance request packet (adjust CRC if needed)
request = b'\x00\x14\x00\x18\x3d'
ser.write(request)
print("Request sent")

time.sleep(0.5)  # wait for response

response = ser.read(ser.in_waiting or 20)
print("Response (hex):", response.hex())
