#!/bin/bash

# Script to export all duplicator game files into one combined file
# with C-style comment separators

OUTPUT_FILE="duplicator_combined.txt"
SOURCE_DIR="."

# Remove old output file if it exists
rm -f "$OUTPUT_FILE"

# Array of duplicator-related files to export
FILES=(
    "duplicator.c"
    "duplicator.txt"
    "duplicator_font.h"
    "duplicator_game.c"
    "duplicator_game.h"
    "duplicator_test.c"
)

echo "Exporting duplicator game files to $OUTPUT_FILE..."

for file in "${FILES[@]}"; do
    if [ -f "$SOURCE_DIR/$file" ]; then
        echo "Adding $file..."
        
        # Add separator with filename
        echo "/* -----------------------------------------------------------------------" >> "$OUTPUT_FILE"
        echo " * File: $file" >> "$OUTPUT_FILE"
        echo " * ----------------------------------------------------------------------- */" >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"
        
        # Add file content
        cat "$SOURCE_DIR/$file" >> "$OUTPUT_FILE"
        
        # Add blank lines for separation
        echo "" >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"
    else
        echo "Warning: $file not found, skipping..."
    fi
done

echo "Export complete! Output saved to $OUTPUT_FILE"
echo "Total files exported: $(grep -c "File:" "$OUTPUT_FILE")"

