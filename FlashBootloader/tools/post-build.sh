#!/bin/bash

# Paths and filenames
PRIVATE_KEY_FILE="private_key.pem"
INPUT_BINARY="$1"
OUTPUT_BINARY_WITH_PADDING="output_with_pad.bin"
OUTPUT_BINARY_WITH_SIGNATURE="output_with_sig.bin"
TMP_HASH_FILE="hash.sha"
TMP_SIG_FILE="signature.sig"
MAX_SIZE=$((256 * 1024))  # 256 KB

# Check if the input binary exists
if [[ ! -f "$INPUT_BINARY" ]]; then
    echo "Input binary file $INPUT_BINARY does not exist!"
    exit 1
fi

# Check if the private key exists
if [[ ! -f "$PRIVATE_KEY_FILE" ]]; then
    echo "Private key file $PRIVATE_KEY_FILE does not exist!"
    exit 1
fi

# Get the size of the binary file
FILESIZE=$(stat -c%s "$INPUT_BINARY")

echo "Binary file size: $FILESIZE bytes"

# Add padding if the binary size is less than 256 KB
if [[ "$FILESIZE" -lt "$MAX_SIZE" ]]; then
    PADDING_SIZE=$((MAX_SIZE - FILESIZE))
    echo "Padding binary file to 256 KB..."
    cp "$INPUT_BINARY" "$OUTPUT_BINARY_WITH_PADDING"
    dd if=/dev/zero bs=1 count="$PADDING_SIZE" >> "$OUTPUT_BINARY_WITH_PADDING"
else
    echo "No padding required. Copying the binary file."
    cp "$INPUT_BINARY" "$OUTPUT_BINARY_WITH_PADDING"
fi

# Generate the SHA-256 hash of the padded binary (for signing)
echo "Generating SHA-256 hash..."
openssl dgst -sha256 -binary "$OUTPUT_BINARY_WITH_PADDING" > "$TMP_HASH_FILE"

# Sign the hash using RSA with the private key
echo "Signing the hash with RSA..."
openssl dgst -sha256 -sign "$PRIVATE_KEY_FILE" -out "$TMP_SIG_FILE" "$OUTPUT_BINARY_WITH_PADDING"
echo "File signature:"
hexdump -C "$TMP_SIG_FILE"

# Combine the padded binary and signature into one file (no hash appended)
echo "Combining binary with signature..."
cat "$OUTPUT_BINARY_WITH_PADDING" "$TMP_SIG_FILE" > "$OUTPUT_BINARY_WITH_SIGNATURE"

echo "Final binary with signature: $OUTPUT_BINARY_WITH_SIGNATURE"

# Clean up temporary files
rm "$TMP_HASH_FILE" "$TMP_SIG_FILE"

echo "Post-build process completed successfully!"

