#!/bin/bash
echo "CHANGE FLAGS IN BASH"

cd src

echo "----------------------------"

# $? = 0 se compilou bem
# $? = 2 otherwise
make 

if [ $? -eq 0 ] ; then
  ./simpledu -l ../TestDir -a --block-size=4096 --max-depth=3 -L -S
  make clean
else
  echo "COMPILATION ERROR";
fi
