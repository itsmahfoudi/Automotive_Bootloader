import serial

# Windows COM port (e.g., COM4)
serial_port = 'COM3'
baud_rate = 2000000  # Adjust to match CANUSB_TTY_BAUD_RATE_DEFAULT

# Example CAN frame structure based on provided C code
can_frame = bytearray([0xAA, 0x55, 0x12, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x55])

# Open the serial port on Windows
with serial.serial(serial_port, baud_rate, timeout=1) as ser:
    # Write CAN frame to serial port
    ser.write(can_frame)
    print(f"Sent CAN frame: {can_frame.hex()}")

    # Read response (if applicable)
    response = ser.read(64)  # Adjust buffer size if necessary
    print(f"Received response: {response.hex()}")

