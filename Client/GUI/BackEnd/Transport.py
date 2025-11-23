from abc import ABC, abstractmethod
import time, serial

class Transport(ABC):
    """Abstract base class for UDS transport layers."""

    @abstractmethod
    def connect(self):
        """Establish connection."""
        pass

    @abstractmethod
    def disconnect(self):
        """Terminate connection."""
        pass

    @abstractmethod
    def send(self, data: bytes) -> None:
        """Sends data over the transport layer."""
        pass

    @abstractmethod
    def receive(self, timeout: float) -> bytes:
        """Receives data from the transport layer.
        Should raise TimeoutError or a specific exception on timeout."""
        pass

class CanTp(Transport):
    def __init__(self, can_interface: str, rx_id: int, tx_id: int):
        # to be implemented after
        pass

    def connect(self):
        # to be implemented after
        pass

    def disconnect(self):
        # to be implemented after
        pass

    def send(self, data: bytes) -> None:
        # to be implemented after
        pass

    def receive(self, timeout: float) -> bytes:
        # to be implemented after
        pass

"""
the first version of our bootloader will be relying on serial communication using uart
CAN support will be added in the next version
"""


class UartTp(Transport):
    def __init__(self, serial_interface: str, baudrate: int = 115200):
        self.serial_interface = serial_interface
        self.baudrate = baudrate
        self.ser = None

    def connect(self):
        try:
            self.ser = serial.Serial(self.serial_interface, self.baudrate, timeout=0.1)
            print(f"Connected to {self.serial_interface} at {self.baudrate}")
        except serial.SerialException as e:
            print(f"Error opening serial port: {e}")
            raise ConnectionError(f"Could not open serial port {self.serial_interface}") from e
    
    def disconnect(self):
        if self.ser and self.ser.is_open:
            self.ser.close()
            print("Serial port closed")

    def send(self, data: bytes) -> None:
        if self.ser and self.ser.is_open:
            self.ser.write(data)
        else:
            raise ConnectionError("Serial port not open")
    
    def receive(self, timeout: float) -> bytes:
        if not (self.ser and self.ser.is_open):
            raise ConnectionError("Serial port not open")
        
        self.ser.timeout = timeout
        # Read 8 bytes (CAN simulation)
        data = self.ser.read(8)
        if not data:
            raise TimeoutError("No data received from serial port")
        return data