#!/bin/bash

# File paths
PRIVATE_KEY_FILE="private_key.pem"
PUBLIC_KEY_PEM_FILE="public_key.pem"
HEADER_FILE="public_key.h"

# Generate the RSA private key
echo "Generating RSA private key..."
openssl genpkey -algorithm RSA -out "$PRIVATE_KEY_FILE" -pkeyopt rsa_keygen_bits:2048

# Extract the public key
echo "Extracting public key..."
openssl rsa -pubout -in "$PRIVATE_KEY_FILE" -out "$PUBLIC_KEY_PEM_FILE"

# Extract the modulus from the public key
echo "Extracting the modulus..."
openssl rsa -pubin -in "$PUBLIC_KEY_PEM_FILE" -noout -modulus > temp.txt
awk '{print substr($0, 9)}' temp.txt > modulus.txt

# Extract the exponent from the public key
echo "Extracting the exponent..."
openssl rsa -pubin -in "$PUBLIC_KEY_PEM_FILE" -text -noout | grep "Exponent:" | awk '{print $2}' > exponent.txt

# Generate the C header file
echo "Generating C header file..."
{
    echo "// public_key.h"
    echo "#ifndef PUBLIC_KEY_H"
    echo "#define PUBLIC_KEY_H"
    echo ""

    # Convert modulus from hex to C array format
    echo -n "const unsigned char modulus[] = {"
    first=true
    while read -r -n2 byte; do
        if [ -n "$byte" ]; then
            if [ "$first" = true ]; then
                echo -n "0x$byte"
                first=false
            else
                echo -n ", 0x$byte"
            fi
        fi
    done < modulus.txt
    echo "};"

    # Convert the exponent to C array format and handle the endianess
    exponent=$(cat exponent.txt)
    hex_exp=$(printf '%04X' "$exponent") # Convert the exponent to a 4-digit hex string
    echo -n "const unsigned char exponent[] = {"
    # Correct endianess: Split hex_exp into bytes and print them in little-endian order
    for (( i=${#hex_exp}; i>0; i-=2 )); do
        echo -n "0x${hex_exp:i-2:2}"
        if (( i > 2 )); then
            echo -n ", "
        fi
    done
    echo "};"
    echo ""
    echo "#endif // PUBLIC_KEY_H"
} > "$HEADER_FILE"

# Clean up temporary files
rm temp.txt exponent.txt modulus.txt 
mv public_key.h ../CM7/Core/Inc/

echo "Key generation and conversion completed successfully!"

