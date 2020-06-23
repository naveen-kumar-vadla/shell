#! /bin/bash

directory=$1
executable=`echo $directory | tr '/' '_'`_main.out

mkdir -p outputs
rm -rf $directory/*.o
gcc -c $directory/*.c
gcc -o outputs/$executable *.o && outputs/$executable
if [[ $? -ne 0 ]]; then
  rm -rf *.o
  exit 1
fi
rm -rf *.o