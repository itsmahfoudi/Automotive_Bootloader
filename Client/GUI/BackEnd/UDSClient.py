from enum import Enum
import time

# Use relative imports within the same package/folder structure
from .Transport import Transport, TimeoutError # Assuming TimeoutError is raised by Transport
from .UDSRequest import (
    UDSRequest, DiagnosticSessionControlRequest, SecurityAccessRequest,
    RequestDownloadRequest, TransferDataRequest, RequestTransferExitRequest,
    ECUResetRequest, RoutineControlRequest
)
from .UDSResponse import UDSResponse
from .UDSFrame import UDSFrame

# Session supported types
class SessionLevel(Enum):
    DEFAULT = DiagnosticSessionControlRequest.SESSION_DEFAULT
    PROGRAMMING = DiagnosticSessionControlRequest.SESSION_PROGRAMMING
    EXTENDED = DiagnosticSessionControlRequest.SESSION_EXTENDED
    SAFETY = DiagnosticSessionControlRequest.SESSION_SAFETY # Keep defined

# Simplified SecurityLevel Enum
class SecurityLevel(Enum):
    LOCKED = 0
    UNLOCKED = 1 # Single unlocked level


class UDSClient:
    """High-level UDS client for interacting with an ECU."""

    # Default timeout for waiting for a response in seconds
    DEFAULT_TIMEOUT = 2.0
    # Default delay after sending a request before attempting to receive
    DEFAULT_POST_SEND_DELAY = 0.01
    # Default number of retries for pending responses (NRC 0x78)
    DEFAULT_PENDING_RETRIES = 5
    # Default timeout specifically for pending responses
    DEFAULT_PENDING_TIMEOUT = 5.0


    def __init__(self, transport: Transport, timeout: float = DEFAULT_TIMEOUT):
        if not isinstance(transport, Transport):
            raise TypeError("transport must be an instance of a Transport subclass")
        self.transport = transport
        self.timeout = timeout
        self.post_send_delay = self.DEFAULT_POST_SEND_DELAY
        self.pending_retries = self.DEFAULT_PENDING_RETRIES
        self.pending_timeout = self.DEFAULT_PENDING_TIMEOUT

        # State variables - Initialize directly to default/locked state
        self.current_session: SessionLevel = SessionLevel.DEFAULT
        self.security_level: SecurityLevel = SecurityLevel.LOCKED
        print(f"Client Initialized. Assumed State: Session={self.current_session.name}, Security={self.security_level.name}")

    def connect(self):
        """Connects the underlying transport. Assumes ECU starts in Default session."""
        self.transport.connect()
        # No need to explicitly set default session, client assumes it starts there.
        # Reset client state variables to assumed initial state upon connection
        self.current_session = SessionLevel.DEFAULT
        self.security_level = SecurityLevel.LOCKED
        print("Transport connected. Client state reset to assumed initial: DEFAULT/LOCKED.")


    def disconnect(self):
        """Disconnects the underlying transport and resets state to assumed initial."""
        try:
            self.transport.disconnect()
        except Exception as e:
             print(f"Warning: Error during transport disconnect: {e}")
        finally:
            # Reset state to assumed initial state after disconnect attempt
            self.current_session = SessionLevel.DEFAULT
            self.security_level = SecurityLevel.LOCKED
            print("Disconnected. Client state reset to assumed initial: DEFAULT/LOCKED.")

    def send_request(self, request: UDSRequest, timeout: float | None = None) -> UDSResponse:
        """Builds, sends, receives, and parses a UDS request."""
        if timeout is None:
            timeout = self.timeout

        frame_to_send = request.build_frame()
        raw_request_bytes = frame_to_send.build_frame()
        original_sid = frame_to_send.service_id

        print(f"CLIENT SEND -> {frame_to_send}")
        self.transport.send(raw_request_bytes)

        time.sleep(self.post_send_delay)

        retry_count = 0
        while retry_count <= self.pending_retries:
            current_timeout = self.pending_timeout if retry_count > 0 else timeout
            try:
                print(f"CLIENT RECV <- (Waiting {current_timeout:.2f}s)")
                raw_response_bytes = self.transport.receive(current_timeout)
                print(f"CLIENT RECV RAW <- {raw_response_bytes.hex()}")

                response = UDSResponse.parse(raw_response_bytes, original_sid)
                print(f"CLIENT PARSED <- {response}")

                if not response.is_positive and response.negative_response_code == 0x78:
                    retry_count += 1
                    if retry_count > self.pending_retries:
                         print("Max pending retries exceeded.")
                         raise TimeoutError(f"UDS response timed out for SID 0x{original_sid:02X} after {self.pending_retries} pending retries.")
                    print(f"Received Pending (0x78), retrying ({retry_count}/{self.pending_retries})...")
                    time.sleep(self.pending_timeout / 5)
                    continue

                if response.is_positive:
                    self._update_state(request, response)

                return response

            except TimeoutError:
                print(f"CLIENT TIMEOUT <- after {current_timeout:.2f}s")
                raise TimeoutError(f"UDS response timed out for SID 0x{original_sid:02X}")
            except ValueError as e:
                 print(f"CLIENT PARSE ERROR <- {e}")
                 raise ConnectionError(f"Failed to parse response for SID 0x{original_sid:02X}: {e}") from e
            except ConnectionError as e:
                 print(f"CLIENT TRANSPORT ERROR <- {e}")
                 raise

        raise TimeoutError(f"UDS response timed out unexpectedly for SID 0x{original_sid:02X}.")


    def _update_state(self, request: UDSRequest, response: UDSResponse):
        """Updates internal client state based on successful requests."""
        if not response.is_positive:
            return # Only update state on success

        if isinstance(request, DiagnosticSessionControlRequest):
            try:
                # Update state based on the *requested* session type
                new_session = SessionLevel(request.session_type)
                if new_session != self.current_session:
                     self.current_session = new_session
                     print(f"State Update: Session changed to {self.current_session.name}")
                     # Changing session often resets security level
                     if self.security_level != SecurityLevel.LOCKED:
                         self.security_level = SecurityLevel.LOCKED
                         print(f"State Update: Security level reset to {self.security_level.name} due to session change.")
            except ValueError:
                 # This case should ideally not happen if request validation is correct
                 # If it does, revert to default as state is uncertain
                 print(f"Warning: Unknown session type {request.session_type} acknowledged. Reverting state to DEFAULT/LOCKED.")
                 self.current_session = SessionLevel.DEFAULT
                 self.security_level = SecurityLevel.LOCKED


        elif isinstance(request, SecurityAccessRequest):
            if request.is_key_request: # If we just sent a key successfully
                 # Transition to the single UNLOCKED state
                 if self.security_level != SecurityLevel.UNLOCKED:
                     self.security_level = SecurityLevel.UNLOCKED
                     print(f"State Update: Security level changed to {self.security_level.name}")
            # No state change when just requesting seed

        elif isinstance(request, ECUResetRequest):
            # ECU Reset always reverts session and security level to default/locked
            print(f"State Update: ECU Reset acknowledged. Resetting session and security level to default/locked.")
            self.current_session = SessionLevel.DEFAULT
            self.security_level = SecurityLevel.LOCKED
            # Note: Server performs reset AFTER sending response. Connection likely lost.


    # --- Service-specific public methods ---

    def diagnostic_session_control(self, session_type: int, timeout: float | None = None) -> UDSResponse:
        """Sends a Diagnostic Session Control (0x10) request."""
        request = DiagnosticSessionControlRequest(session_type=session_type)
        return self.send_request(request, timeout=timeout)

    def ecu_reset(self, reset_type: int, timeout: float | None = None) -> UDSResponse:
        """Sends an ECU Reset (0x11) request. Handles expected disconnect/reconnect."""
        request = ECUResetRequest(reset_type=reset_type)
        effective_timeout = timeout if timeout is not None else self.timeout * 1.5 # Allow longer time

        try:
            print("Sending ECU Reset request...")
            response = self.send_request(request, timeout=effective_timeout)

            if response.is_positive:
                print("ECU Reset positive response received. Server is resetting.")
                # Server resets AFTER response. Connection will drop.
                # Disconnect transport cleanly from client side.
                print("Disconnecting transport due to expected ECU reset...")
                self.disconnect() # Resets state to DEFAULT/LOCKED internally

                # Optional: Add a delay before allowing reconnection attempt
                print("Waiting briefly for ECU to reset...")
                time.sleep(3.0) # Adjust delay as needed for target ECU

                print("Attempting to reconnect transport after reset...")
                try:
                    self.connect() # Reconnect and reset state
                    print("Reconnected successfully after ECU reset.")
                except Exception as recon_e:
                    print(f"Failed to reconnect after ECU reset: {recon_e}")
                    # State is already reset by disconnect()
                    raise ConnectionError("Failed to reconnect after ECU reset") from recon_e

            return response # Return the original response (positive or negative)

        except TimeoutError as e:
            print("ECU Reset timed out. Server might have reset without responding.")
            # Assume reset happened, disconnect and try reconnecting
            print("Disconnecting transport due to reset timeout...")
            self.disconnect() # Resets state to DEFAULT/LOCKED

            print("Waiting briefly assuming ECU reset...")
            time.sleep(3.0) # Adjust delay

            print("Attempting to reconnect transport after reset timeout...")
            try:
                self.connect() # Reconnect and reset state
                print("Reconnected successfully after ECU reset timeout.")
            except Exception as recon_e:
                print(f"Failed to reconnect after ECU reset timeout: {recon_e}")
                raise ConnectionError("Failed to reconnect after ECU reset timeout") from recon_e

            # Re-raise the original timeout error after attempting reconnect
            raise e
        except Exception as e:
             # Catch other errors during send/receive
             print(f"An error occurred during ECU Reset process: {e}")
             # Ensure state is reset even if something else went wrong
             self.disconnect()
             raise


    def security_access_request_seed(self, timeout: float | None = None) -> UDSResponse:
        """Sends a Security Access (0x27) request to get a seed (uses level 0x01)."""
        request = SecurityAccessRequest(level=SecurityAccessRequest.REQUEST_SEED_LEVEL)
        return self.send_request(request, timeout=timeout)

    def security_access_send_key(self, key: bytes, timeout: float | None = None) -> UDSResponse:
        """Sends a Security Access (0x27) request with a key (uses level 0x02)."""
        request = SecurityAccessRequest(level=SecurityAccessRequest.SEND_KEY_LEVEL)
        request.set_key(key)
        return self.send_request(request, timeout=timeout)

    def request_download(self, memory_address: int, memory_size: int, data_format: int = 0x00, timeout: float | None = None) -> UDSResponse:
        """Sends a Request Download (0x34) request."""
        request = RequestDownloadRequest(memory_address=memory_address, memory_size=memory_size, data_format=data_format)
        return self.send_request(request, timeout=timeout)

    def transfer_data(self, block_sequence_counter: int, data: bytes, timeout: float | None = None) -> UDSResponse:
        """Sends a Transfer Data (0x36) request."""
        request = TransferDataRequest(block_sequence_counter=block_sequence_counter, data=data)
        return self.send_request(request, timeout=timeout)

    def request_transfer_exit(self, transfer_request_parameter: bytes = b'', timeout: float | None = None) -> UDSResponse:
        """Sends a Request Transfer Exit (0x37) request."""
        request = RequestTransferExitRequest(transfer_request_parameter=transfer_request_parameter)
        return self.send_request(request, timeout=timeout)

    def routine_control_start_erase(self, address: int, size_kb: int, timeout: float | None = None) -> UDSResponse:
        """Sends a Routine Control (0x31) request to start memory erase."""
        request = RoutineControlRequest(
            routine_control_type=RoutineControlRequest.START_ROUTINE,
            routine_id=RoutineControlRequest.ERASING_MEM
        )
        request.add_erase_parameters(address=address, size_kb=size_kb)
        effective_timeout = timeout if timeout is not None else self.timeout * 5.0
        return self.send_request(request, timeout=effective_timeout)

