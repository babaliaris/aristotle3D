#!/bin/bash

# Run Premake to generate the compile commands
echo "Generating compile commands..."
premake5 export-compile-commands

# Check if the file was actually generated
TARGET="compile_commands/debug_x64.json"
LINK_NAME="compile_commands.json"

if [ -f "$TARGET" ]; then
    # Create the symlink
    # -f (force) removes any existing link so it can be recreated safely
    ln -sf "$TARGET" "$LINK_NAME"
    echo "Success: Linked $TARGET to $LINK_NAME"
else
    echo "Error: $TARGET not found. Check your Premake config."
    exit 1
fi

