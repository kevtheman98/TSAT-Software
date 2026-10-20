import struct
import serial
import cv2
import numpy as np

SERIAL_PORT = "/dev/ttyUSB0"
BAUD_RATE = 115200
MARKER = b"\xFF\xAA\x55\xFF"
MAX_FRAME_SIZE = 200_000

ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2)
print("Waiting for frames... press 'q' in the video window, or Ctrl+C here, to quit.")

buffer = b""
try:
    while True:
        chunk = ser.read(ser.in_waiting or 1)
        if not chunk:
            print("(no data received in the last 2 seconds)")
        buffer += chunk

        start = buffer.find(MARKER)

        # Print any text that arrives before a frame (boot messages, errors)
        text_end = start if start != -1 else len(buffer)
        if start == -1 and len(buffer) > 0 and all(b < 128 for b in buffer[:text_end]):
            text = buffer[:text_end].decode("utf-8", errors="ignore").strip()
            if text:
                print("BOARD:", text)
                buffer = b""
            continue
        if start > 0:
            text = buffer[:start].decode("utf-8", errors="ignore").strip()
            if text:
                print("BOARD:", text)
            buffer = buffer[start:]
            start = 0

        if start == -1 or len(buffer) < start + 8:
            continue

        length = struct.unpack("<I", buffer[start + 4:start + 8])[0]
        print(f"Frame header found, length = {length}")

        if length == 0 or length > MAX_FRAME_SIZE:
            buffer = buffer[start + 4:]
            continue

        frame_end = start + 8 + length
        if len(buffer) < frame_end:
            continue

        jpeg_bytes = buffer[start + 8:frame_end]
        buffer = buffer[frame_end:]

        frame = cv2.imdecode(np.frombuffer(jpeg_bytes, dtype=np.uint8), cv2.IMREAD_COLOR)
        if frame is None:
            print("JPEG decode failed")
            continue

        cv2.imshow("Arducam Live Feed", frame)
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break
except KeyboardInterrupt:
    pass

ser.close()
cv2.destroyAllWindows()