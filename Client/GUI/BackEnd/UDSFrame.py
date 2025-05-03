import UDSResponse

class UDSFrame:
    """Represents a basic UDS Frame (Protocol Data Unit)."""
    def __init__(self, service_id: int, data: bytearray = bytearray()):
        if not (0x00 <= service_id <= 0xFF):
             raise ValueError("Service ID must be a byte (0x00-0xFF)")
        self.service_id = service_id
        self.data = data # Payload (sub-function and parameters)

    def build_frame(self) -> bytes:
        """Constructs the raw byte sequence for the frame."""
        return bytes([self.service_id]) + self.data

    @classmethod
    def parse_frame(cls, raw_frame: bytes):
        # Parses raw bytes into a UDSFrame object

        if not raw_frame:
            raise ValueError("Cannot parse empty frame")
        service_id = raw_frame[0]
        data = bytearray(raw_frame[1:])
        return cls(service_id=service_id, data=data)

    def __repr__(self):
        return f"UDSFrame(SID=0x{self.service_id:02X}, Data={self.data.hex()})"

