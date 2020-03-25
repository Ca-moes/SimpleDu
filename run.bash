#!/bin/bash

echo "Mudar as flags neste -bash"
echo "__________________________"
echo

cd src

if gcc -Wall -o simpledu simpledu.c; then
./simpledu --block-size=512 -l --max-depth=3 -L -S
else 
echo "COMPILATION ERROR";
fi 

# apaga todos os binarios criados por gcc
find . -type f  ! -name "*.*"  -delete