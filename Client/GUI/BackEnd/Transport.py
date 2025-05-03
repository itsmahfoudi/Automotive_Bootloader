from abc import ABC, abstractmethod
import time

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
        # to be implemented
        pass

    def connect(self):
        # To be implemented after
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