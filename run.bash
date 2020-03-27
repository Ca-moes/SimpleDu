#!/bin/bash

echo "CHANGE FLAGS IN BASH"

cd src

make 
echo "----------------------------"


# $? = 0 se compilou bem
# $? = 2 otherwise

if [ $? -eq 0 ] ; then
  ./simpledu.exe -l ../TestDir -a --block-size=4096 --max-depth=3 -L -S
else
  echo "COMPILATION ERROR";
fi

echo "----------------------------"
make clean

# apaga todos os binarios criados por gcc
# find . -type f  ! -name "*.*"  -delete
# apaga os .o
# find . -type f -iname \*.o -delete