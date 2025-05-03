class UDSResponse:
    """Represents a parsed UDS response."""
    def __init__(self, is_positive: bool, request_sid: int, data: bytearray, negative_response_code: int = 0):
        self.is_positive = is_positive
        self.request_sid = request_sid # SID of the original request
        self.data = data # Response data (excluding SID or NRC info)
        self.negative_response_code = negative_response_code # 0 if positive, NRC otherwise

    @classmethod
    def parse(cls, raw_bytes: bytes, original_request_sid: int):
        """Parses raw bytes received from the transport layer into a UDSResponse."""
        if not raw_bytes:
            raise ValueError("Received empty response")

        response_sid = raw_bytes[0]
        payload = bytearray(raw_bytes[1:]) # Full payload after SID

        if response_sid == 0x7F and len(payload) >= 2 and payload[0] == original_request_sid:
            # Negative Response (Standard format: 0x7F, Request SID, NRC, optional data)
            nrc = payload[1]
            response_data = payload[2:] # Any data after NRC
            # Check for pending NRC specifically
            is_pending = (nrc == 0x78)
            # Return standard response object, client handles pending logic
            return cls(is_positive=False, request_sid=original_request_sid, data=response_data, negative_response_code=nrc)

        elif response_sid == (original_request_sid + 0x40):
            # Positive Response - Handle SID-specific formats based on server code
            response_data = bytearray() # Default to empty data

            if original_request_sid == 0x10: # Diagnostic Session Control
                # Server sends 0x50 + next_session + padding
                # Client state update relies on the *requested* session being acknowledged.
                # We extract the echoed byte just in case it's needed, but ignore padding.
                if len(payload) >= 1:
                    response_data = payload[0:1] # Capture the first byte (next session)
                else:
                    # Even if padded, should have at least the next session byte
                    print(f"Warning: Positive response for SID 0x10 unexpectedly short: {payload.hex()}")
                    # Allow processing to continue, data will be empty or partial

            elif original_request_sid == 0x11: # ECU Reset
                # Server sends 0x51 + echoed_sub_function
                if len(payload) >= 1:
                    response_data = payload[0:1] # Echoed sub-function
                else:
                     # This would be an error according to server code
                     raise ValueError(f"Positive response for SID 0x11 too short: {payload.hex()}")

            elif original_request_sid == 0x37: # Request Transfer Exit
                 # Server sends only 0x77
                 response_data = bytearray() # No data expected

            elif original_request_sid == 0x31: # Routine Control
                 # Assuming positive response is 0x71 + echoed_sub_function + echoed_routine_id (3 bytes total)
                 if len(payload) >= 3:
                     response_data = payload[0:3]
                 else:
                     # Allow processing but log warning, data might be incomplete
                     print(f"Warning: Positive response for SID 0x31 may be short: {payload.hex()}")
                     response_data = payload # Store whatever was received

            # Add other SIDs based on server behavior if known...
            # Example: Security Access Seed (0x27, level odd) -> 0x67 + level + seed
            elif original_request_sid == 0x27 and (payload[0] % 2 != 0 if len(payload)>0 else False): # Seed request
                 if len(payload) >= 1:
                     response_data = payload # Includes echoed level + seed
                 else:
                     raise ValueError(f"Positive response for SID 0x27 (seed) too short: {payload.hex()}")
            # Example: Security Access Key (0x27, level even) -> 0x67 + level
            elif original_request_sid == 0x27 and (payload[0] % 2 == 0 if len(payload)>0 else False): # Key send
                 if len(payload) >= 1:
                     response_data = payload[0:1] # Only echoed level expected
                 else:
                     raise ValueError(f"Positive response for SID 0x27 (key) too short: {payload.hex()}")

            else:
                 # Default for unknown positive responses: Assume entire payload is data
                 # This might include server padding (e.g., 0xFF)
                 response_data = payload
                 print(f"Warning: Parsing unknown positive response SID 0x{original_request_sid:02X}. Data: {response_data.hex()}")


            return cls(is_positive=True, request_sid=original_request_sid, data=response_data, negative_response_code=0)

        else:
            # Unexpected response SID or format
            raise ValueError(f"Unexpected response. Raw: {raw_bytes.hex()}. Request SID: 0x{original_request_sid:02X}. Response SID: 0x{response_sid:02X}")

    def __repr__(self):
        if self.is_positive:
            return f"UDSResponse(Positive, ReqSID=0x{self.request_sid:02X}, Data={self.data.hex()})"
        else:
            nrc_str = f"NRC=0x{self.negative_response_code:02X}"
            if self.negative_response_code == 0x78:
                nrc_str += " (Pending)"
            return f"UDSResponse(Negative, ReqSID=0x{self.request_sid:02X}, {nrc_str}, Data={self.data.hex()})"
