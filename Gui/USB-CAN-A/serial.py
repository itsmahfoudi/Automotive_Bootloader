from serial import Serial
import time

# Configure the COM port and baud rate
serial_port = '/dev/ttyACM0'  # Update if your COM port is different
baud_rate = 115200    # Adjust this to match the STM32 UART baud rate
timeout = 1           # Timeout for reading from the serial port

# Open the serial port
ser = serial.Serial(serial_port, baud_rate, timeout=100)
#with serial.Serial(serial_port, baud_rate, timeout=timeout) as ser:
    # Function to send data
def send_data(data):
    if ser.is_open:
        ser.write(data.encode())  # Convert string to bytes before sending
        print("Sent: {data}")
    
    # Function to receive data
def receive_data():
    if ser.is_open:
        response = ser.readline()  # Read a line of data (you can also use ser.read() for raw bytes)
        if response:
            print(f"Received: {response.decode().strip()}")  # Decode bytes to string and strip newlines

    # Example usage
while True:
        # Send data to STM32
    data_to_send = "Hello STM32\n"  # Add newline if the STM32 expects it as a terminator
    send_data(data_to_send)

        # Wait a moment before reading the response
    time.sleep(1)

        # Read response from STM32
    receive_data()

        # Delay before next transmission
    time.sleep(2)

