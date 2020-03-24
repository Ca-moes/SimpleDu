#!/bin/bash

echo "Mudar as flags neste -bash"
echo "__________________________"
echo

if gcc -Wall -o simpledu simpledu.c; then
./simpledu -a -aa
else 
echo "COMPILATION ERROR";
fi 

