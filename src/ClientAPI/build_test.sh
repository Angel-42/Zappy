#!/bin/bash
set -e

SRC_DIR="$(dirname "$0")"
OUT_BIN="clientapi_test"

# Find all .cpp files in this directory and subdirectories
SRCS=$(find "$SRC_DIR" -name '*.cpp')

# Compile
g++ -std=c++17 -pthread -I"$SRC_DIR" $SRCS -o "$SRC_DIR/$OUT_BIN"
echo "Built $OUT_BIN successfully." 