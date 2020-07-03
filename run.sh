#! /bin/bash

executable=$1.out

mkdir -p bin
mkdir -p outputs
rm -rf *.o
gcc -c *.c
gcc -o outputs/$executable *.o && outputs/$executable
if [[ $? -ne 0 ]]; then
  rm -rf *.o
  exit 1
fi
rm -rf *.o