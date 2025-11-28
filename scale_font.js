#!/usr/bin/env node

/**
 * scale_font.js - Scale 8x8 font data to 16x16 for Atari 8-bit
 * 
 * Usage: node scale_font.js
 * 
 * Reads duplicator_font.h and generates duplicator_graphics_16x16.h
 * with pre-scaled 16x16 tile data (4 characters per tile, 32 bytes each)
 */

const fs = require('fs');
const path = require('path');

// Read the input font file
const inputFile = path.join(__dirname, 'duplicator_font.h');
const fontData = fs.readFileSync(inputFile, 'utf8');

// Extract the duplicator_graphics array
const arrayMatch = fontData.match(/static unsigned char duplicator_graphics\[\] = \{([\s\S]*?)\};/);
if (!arrayMatch) {
    console.error('Could not find duplicator_graphics array in input file');
    process.exit(1);
}

// Parse the byte values - remove comments first
let arrayContent = arrayMatch[1];
// Remove single-line comments
arrayContent = arrayContent.replace(/\/\/.*$/gm, '');
// Remove multi-line comments
arrayContent = arrayContent.replace(/\/\*[\s\S]*?\*\//g, '');

const bytes = [];
const byteMatches = arrayContent.matchAll(/0[xb][0-9A-Fa-f]+|\d+/g);
for (const match of byteMatches) {
    let value = match[0];
    if (value.startsWith('0b')) {
        bytes.push(parseInt(value.substring(2), 2));
    } else if (value.startsWith('0x')) {
        bytes.push(parseInt(value.substring(2), 16));
    } else {
        bytes.push(parseInt(value, 10));
    }
}

console.log(`Found ${bytes.length} bytes in duplicator_graphics array`);
console.log(`This represents ${bytes.length / 8} tiles (8 bytes each)`);

/**
 * Scale one 8x8 tile (8 bytes) to 16x16 (32 bytes = 4 chars)
 * Each pixel becomes 2x2 pixels
 */
function scale8x8to16x16(src8x8) {
    const tl = new Array(8).fill(0);  // Top-left char
    const tr = new Array(8).fill(0);  // Top-right char
    const bl = new Array(8).fill(0);  // Bottom-left char
    const br = new Array(8).fill(0);  // Bottom-right char

    // Process each row of the 8x8 source
    for (let row = 0; row < 8; row++) {
        const srcByte = src8x8[row];

        // Scale horizontally: each bit becomes 2 bits
        // Bits 7-4 go to left half, bits 3-0 go to right half
        let leftHalf = 0;
        let rightHalf = 0;

        // Process each bit in the source byte
        for (let bit = 0; bit < 8; bit++) {
            if (srcByte & (1 << (7 - bit))) {
                // Pixel is set - set 2 bits in output
                if (bit < 4) {
                    // Left half of source -> left char (bits 7-4 become bits 7-0)
                    leftHalf |= (3 << (6 - bit * 2));
                } else {
                    // Right half of source -> right char (bits 3-0 become bits 7-0)
                    rightHalf |= (3 << (6 - (bit - 4) * 2));
                }
            }
        }

        // Each source row becomes 2 destination rows (vertical doubling)
        if (row < 4) {
            // Top half -> top-left and top-right chars
            tl[row * 2] = leftHalf;
            tl[row * 2 + 1] = leftHalf;
            tr[row * 2] = rightHalf;
            tr[row * 2 + 1] = rightHalf;
        } else {
            // Bottom half -> bottom-left and bottom-right chars
            bl[(row - 4) * 2] = leftHalf;
            bl[(row - 4) * 2 + 1] = leftHalf;
            br[(row - 4) * 2] = rightHalf;
            br[(row - 4) * 2 + 1] = rightHalf;
        }
    }

    // Combine all 4 chars into one array
    return [...tl, ...tr, ...bl, ...br];
}

// Tile names and their offsets in the original array
// Order matches the character codes in duplicator_font_16x16.h
// Using screen codes 0x00-0x7F (not charset codes)
const tiles = [
    { name: 'Wall', offset: 8, charCode: 0x00 },
    { name: 'Player', offset: 0, charCode: 0x04 },
    { name: 'Crate', offset: 16, charCode: 0x08 },
    { name: 'Key', offset: 24, charCode: 0x0C },
    { name: 'Door', offset: 32, charCode: 0x10 },
    { name: 'Enemy', offset: 40, charCode: 0x14 },
    { name: 'Hole A', offset: 48, charCode: 0x18 },
    { name: 'Hole B', offset: 56, charCode: 0x1C },
    { name: 'Plate A', offset: 64, charCode: 0x20 },
    { name: 'Plate B', offset: 72, charCode: 0x24 },
    { name: 'Gate A', offset: 80, charCode: 0x28 },
    { name: 'Gate B', offset: 88, charCode: 0x2C },
    { name: 'Exit A', offset: 96, charCode: 0x30 },
    { name: 'Exit B', offset: 104, charCode: 0x34 },
    { name: 'Exit C', offset: 208, charCode: 0x38 },
    { name: 'Floor', offset: 112, charCode: 0x3C },
    { name: 'Gate A Open', offset: 216, charCode: 0x40 },
    { name: 'Gate B Open', offset: 224, charCode: 0x44 },
    { name: 'Door Open', offset: 232, charCode: 0x48 },
    { name: 'Hole A Filled', offset: 240, charCode: 0x4C },
    { name: 'Hole B Filled', offset: 248, charCode: 0x50 },
    { name: 'Wall Line A', offset: 120, charCode: 0x54 },
    { name: 'Wall Line B', offset: 128, charCode: 0x58 },
    { name: 'Wall Line G', offset: 136, charCode: 0x5C },
    { name: 'Line A', offset: 144, charCode: 0x60 },
    { name: 'Line B', offset: 152, charCode: 0x64 },
    { name: 'Line C', offset: 160, charCode: 0x68 },
    { name: 'Line D', offset: 168, charCode: 0x6C },
    { name: 'Line E', offset: 176, charCode: 0x70 },
    { name: 'Line F', offset: 184, charCode: 0x74 },
    { name: 'Line G', offset: 192, charCode: 0x78 },
    { name: 'Line H', offset: 200, charCode: 0x7C }
];

// Generate output
let output = `/* duplicator_graphics_16x16.h - Pre-scaled 16x16 graphics */\n`;
output += `/* Generated by scale_font.js - DO NOT EDIT MANUALLY */\n\n`;
output += `#ifndef DUPLICATOR_GRAPHICS_16X16_H\n`;
output += `#define DUPLICATOR_GRAPHICS_16X16_H\n\n`;
output += `/* Each tile is 32 bytes (4 characters x 8 bytes) */\n`;
output += `/* Format: TL (8 bytes), TR (8 bytes), BL (8 bytes), BR (8 bytes) */\n\n`;
output += `static unsigned char duplicator_graphics_16x16[] = {\n`;

for (const tile of tiles) {
    const src = bytes.slice(tile.offset, tile.offset + 8);
    const scaled = scale8x8to16x16(src);

    const charCodeHex = '0x' + tile.charCode.toString(16).toUpperCase();
    output += `    /* ${tile.name} (char ${charCodeHex}-${charCodeHex.slice(0, -1)}${(tile.charCode + 3).toString(16).toUpperCase()}) - 32 bytes */\n`;
    
    // Output in groups of 8 bytes (one character)
    for (let i = 0; i < 32; i += 8) {
        const label = i === 0 ? 'TL' : i === 8 ? 'TR' : i === 16 ? 'BL' : 'BR';
        output += `    /* ${label} */ `;
        output += scaled.slice(i, i + 8).map(b => `0x${b.toString(16).toUpperCase().padStart(2, '0')}`).join(',');
        output += ',\n';
    }
    output += '\n';
}

output += `};\n\n`;
output += `#endif /* DUPLICATOR_GRAPHICS_16X16_H */\n`;

// Write output file
const outputFile = path.join(__dirname, 'duplicator_graphics_16x16.h');
fs.writeFileSync(outputFile, output, 'utf8');

console.log(`\nGenerated ${outputFile}`);
console.log(`Total tiles: ${tiles.length}`);
console.log(`Total bytes: ${tiles.length * 32}`);

