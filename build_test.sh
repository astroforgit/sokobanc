#!/bin/bash
# Build script for duplicator game test suite
# This builds a native executable that can run on modern computers without Atari hardware

set -e  # Exit on error

CC=gcc
CFLAGS="-Wall -Wextra -g -O0 -std=c99"
OUTPUT="duplicator_test"

echo "========================================"
echo "Building Duplicator Game Test Suite"
echo "========================================"

# Compile with -include to force test_conio.h to be included before atari_conio.h
# This allows us to use the test version without modifying duplicator_game.c
$CC $CFLAGS \
    -include test_conio.h \
    -o $OUTPUT \
    test_conio.c \
    duplicator_game.c \
    duplicator_test_runner.c

if [ $? -eq 0 ]; then
    echo ""
    echo "========================================"
    echo "✓ Build successful!"
    echo "Test executable: ./$OUTPUT"
    echo ""
    echo "To run tests:"
    echo "  ./$OUTPUT"
    echo "========================================"
else
    echo ""
    echo "✗ Build failed!"
    exit 1
fi

