#!/bin/bash

# Define file sizes in MiB
sizes=(7 8 9 10)
files=("A.bin" "B.bin" "C.bin" "D.bin")

echo "Generating binary files with random content..."

# Loop through the sizes and filenames
for i in "${!sizes[@]}"; do
    size=${sizes[$i]}
    file=${files[$i]}
    echo "Generating $file with size ${size}MiB..."
    
    # Use `dd` with /dev/urandom to generate random content
    dd if=/dev/urandom of="$file" bs=1M count="$size" status=progress
done

echo "All files generated successfully!"