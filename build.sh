#!/bin/bash

echo "Hello, world"
make

if [ $? -eq 0 ]; then
	./main
fi
