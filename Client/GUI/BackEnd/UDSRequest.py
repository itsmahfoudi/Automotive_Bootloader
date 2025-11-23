from abc import ABC, abstractmethod
from UDSFrame import UDSFrame # Use relative import

class UDSRequest(ABC):
    """Abstract base class for UDS service requests."""
    def __init__(self):
        self._sub_function: int | None = None
        self._data = bytearray() 

    def set_sub_function(self, sub_function: int):
        """Sets the sub-function byte (if applicable)."""
        if not (0x00 <= sub_function <= 0xFF):
             raise ValueError("Sub-function must be a byte (0x00-0xFF)")
        self._sub_function = sub_function
        return self 

    def add_data(self, data: bytes | bytearray | list[int]):
        """Adds data payload bytes."""
        self._data.extend(data)
        return self

    @property
    @abstractmethod
    def service_id(self) -> int:
        """Returns the Service ID for this request type."""
        pass

    def build_frame(self) -> UDSFrame:
        """Builds the UDSFrame for this request."""
        payload = bytearray()
        if self._sub_function is not None: 
            payload.append(self._sub_function & 0x7F) 
        payload.extend(self._data)
        return UDSFrame(service_id=self.service_id, data=payload)

    def __repr__(self):
        # Use property to get service_id
        sid_repr = f"0x{self.service_id:02X}" if self.service_id is not None else "None"
        subfunc_repr = f"0x{self._sub_function:02X}" if self._sub_function is not None else "None"
        return f"{self.__class__.__name__}(SID={sid_repr}, SubFunc={subfunc_repr}, Data={self._data.hex()})"


# --- Concrete Request Classes ---

class DiagnosticSessionControlRequest(UDSRequest):
    SERVICE_ID = 0x10
    SESSION_DEFAULT = 0x01
    SESSION_PROGRAMMING = 0x02
    SESSION_EXTENDED = 0x03

    def __init__(self, session_type: int):
        super().__init__() 
        if not (0x00 <= session_type <= 0x7F): # Allow valid sub-function range
            raise ValueError(f"Invalid session type: {session_type}")
        self.set_sub_function(session_type)
        self.session_type = session_type # Store for client state update

    @property
    def service_id(self) -> int:
        return self.SERVICE_ID

class SecurityAccessRequest(UDSRequest):
    SERVICE_ID = 0x27
    REQUEST_SEED_LEVEL = 0x01 # Standard level to request seed for the single security level
    SEND_KEY_LEVEL = 0x02   # Standard level to send key for the single security level

    def __init__(self, level: int):
        """Initializes Security Access request for a single security level model.
           Use level 0x01 for requesting seed.
           Use level 0x02 for sending key (use set_key).
        """
        super().__init__()
        if level not in [self.REQUEST_SEED_LEVEL, self.SEND_KEY_LEVEL]:
             raise ValueError(f"Invalid security access level for single-level model. Use {self.REQUEST_SEED_LEVEL:#04x} (seed) or {self.SEND_KEY_LEVEL:#04x} (key).")
        self.set_sub_function(level)
        self.level = level
        self.is_key_request = (level == self.SEND_KEY_LEVEL) 

    def set_key(self, key: bytes | bytearray):
        """Sets the key data (only for SEND_KEY_LEVEL)."""
        if not self.is_key_request:
            raise TypeError(f"Cannot set key for a seed request (level {self.REQUEST_SEED_LEVEL:#04x})")
        # Clear previous data if any, set key
        self._data = bytearray(key)
        return self

    @property
    def service_id(self) -> int:
        return self.SERVICE_ID

class RequestDownloadRequest(UDSRequest):
    SERVICE_ID = 0x34 

    def __init__(self, memory_address: int, memory_size: int, data_format: int = 0x00):
        super().__init__()
        addr_len = 4
        size_len = 4
        address_bytes = memory_address.to_bytes(addr_len, 'big')
        size_bytes = memory_size.to_bytes(size_len, 'big')
        alfid = (addr_len << 4) | size_len
        self.add_data([data_format, alfid])
        self.add_data(address_bytes)
        self.add_data(size_bytes)

    @property
    def service_id(self) -> int:
        return self.SERVICE_ID

class TransferDataRequest(UDSRequest):
    SERVICE_ID = 0x36

    def __init__(self, block_sequence_counter: int, data: bytes | bytearray):
        super().__init__()
        if not (0x00 <= block_sequence_counter <= 0xFF):
            raise ValueError("Block sequence counter must be a byte")
        self.set_sub_function(block_sequence_counter)
        self.add_data(data)
        self.block_sequence_counter = block_sequence_counter

    @property
    def service_id(self) -> int:
        return self.SERVICE_ID

class RequestTransferExitRequest(UDSRequest):
    SERVICE_ID = 0x37 
    def __init__(self, transfer_request_parameter: bytes | bytearray = b''):
        super().__init__()
        # No sub-function for 0x37
        self.add_data(transfer_request_parameter)

    @property
    def service_id(self) -> int:
        return self.SERVICE_ID

class ECUResetRequest(UDSRequest):
    """Represents an ECU Reset (0x11) request."""
    SERVICE_ID = 0x11

    # Reset types supported by oussma's server implementation
    HARD_RESET = 0x01
    # KEY_OFF_ON_RESET = 0x02
    SOFT_RESET = 0x03

    def __init__(self, reset_type: int):
        """Initializes the ECU Reset request."""
        super().__init__()
        # Validate against only the types explicitly checked by server
        if reset_type not in [self.HARD_RESET, self.SOFT_RESET]:
             raise ValueError(f"Server only supports Hard (0x01) or Soft (0x03) reset. Invalid type: {reset_type:#04x}")
        self.set_sub_function(reset_type)
        self.reset_type = reset_type # Store for potential client logic

    @property
    def service_id(self) -> int:
        return self.SERVICE_ID

class RoutineControlRequest(UDSRequest):
    """Represents a Routine Control (0x31) request."""
    SERVICE_ID = 0x31

    # Routine Control Types
    START_ROUTINE = 0x01
    STOP_ROUTINE = 0x02
    REQUEST_ROUTINE_RESULTS = 0x03

    # Routine Identifiers
    ERASING_MEM = 0xFF00

    # Memory constraints
    MEM_START = 0x08000000
    MEM_END = 0x08200000 # 2MB Flash range

    def __init__(self, routine_control_type: int, routine_id: int):
        super().__init__()
        if routine_control_type not in [self.START_ROUTINE, self.STOP_ROUTINE, self.REQUEST_ROUTINE_RESULTS]:
            raise ValueError(f"Invalid routineControlType: {routine_control_type:#04x}")
        if not (0x0000 <= routine_id <= 0xFFFF):
            raise ValueError(f"Invalid routineIdentifier: {routine_id:#04x}")

        self.set_sub_function(routine_control_type)
        # Add Routine ID (2 bytes, Big Endian)
        self._data.extend(routine_id.to_bytes(2, 'big'))
        self.routine_control_type = routine_control_type
        self.routine_id = routine_id

    def add_erase_parameters(self, address: int, size_kb: int):
        """Adds parameters specific to StartRoutine(0x01)/Erasing_mem(0xFF00)."""
        if self.routine_control_type != self.START_ROUTINE or self.routine_id != self.ERASING_MEM:
            raise TypeError("Erase parameters only valid for StartRoutine/Erasing_mem")

        # Validate address based on server logic (3 bytes relevant range)
        if not (self.MEM_START <= address <= self.MEM_END):
             raise ValueError(f"Memory address 0x{address:06X} out of server range [0x{self.MEM_START:06X}-0x{self.MEM_END:06X}]")
        # Validate size (1 byte, represents kB)
        if not (0x01 <= size_kb <= 0xFF): # Assuming at least 1KB, max 255KB
             raise ValueError(f"Memory size_kb {size_kb} out of range [1-255]")

        # Add Address (3 bytes, Big Endian, relative to 0x08000000 but sent as offset)
        # Server expects offset: (RxData[4]<<16) | (RxData[5]<<8) | RxData[6]
        # We mask to 24 bits (assuming 0x08xxxxxx -> 0x00xxxxxx)
        self._data.extend((address & 0xFFFFFF).to_bytes(3, 'big'))
        # Add Size (1 byte, in kB)
        self._data.append(size_kb)
        return self

    @property
    def service_id(self) -> int:
        return self.SERVICE_ID

class ReadDataByIDRequest(UDSRequest):
    SERVICE_ID = 0x22

    def __init__(self, data_identifier: int):
        super().__init__()
        if not (0x0000 <= data_identifier <= 0xFFFF):
            raise ValueError(f"Invalid Data Identifier: {data_identifier:#04x}")
        
        # Add DID (2 bytes, Big Endian)
        self._data.extend(data_identifier.to_bytes(2, 'big'))
        self.data_identifier = data_identifier

    @property
    def service_id(self) -> int:
        return self.SERVICE_ID
