#!/bin/bash

# Script to export all duplicator 16x16 game files into one combined file
# with C-style comment separators

OUTPUT_FILE="duplicator_16x16_combined.txt"
SOURCE_DIR="."

# Remove old output file if it exists
rm -f "$OUTPUT_FILE"

# Array of duplicator 16x16 related files to export
FILES=(
    "duplicator_16x16.c"
    "duplicator_conio_16x16.c"
    "duplicator_conio_16x16.h"
    "duplicator_tile_map_16x16.c"
    "duplicator_font_16x16.h"
    "duplicator_graphics_16x16.h"
    "duplicator_game_16x16.c"
    "duplicator_game.h"
)

echo "Exporting duplicator 16x16 game files to $OUTPUT_FILE..."

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

