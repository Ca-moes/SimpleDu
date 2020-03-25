#!/bin/bash

echo "Mudar as flags neste -bash"
echo "__________________________"
echo

cd src

if gcc -Wall -o simpledu simpledu.c; then
./simpledu -a -b -l -L -S
else 
echo "COMPILATION ERROR";
fi 

# apaga todos os binarios criados por gcc
find . -type f  ! -name "*.*"  -delete