#!/bin/bash

file_path="/tmp/ex1.pid"

if [ -f "$file_path" ]; then
    read -r pid < "$file_path"
   
else
    echo "File not found: $file_path"
fi

addr=$(gdb -p $pid -batch -ex "info proc mappings" | awk '/\/dev\/zero/ {print $1}')

gdb -p $pid -batch -ex "set logging enabled off" -ex "call (void)puts((char*)$addr)" -ex "detach" -ex "quit" -q

kill -9 $pid

