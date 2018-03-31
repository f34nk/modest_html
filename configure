#!/bin/bash

current=`pwd`

check=`find libs/Modest \( -name libmodest_static.a -o -name libmodest.so \)`
if [ -z "$check" ]
then
  echo "Compiling Modest..."
  # Modest
  # https://github.com/lexborisov/Modest/blob/master/INSTALL.md
  cd libs/Modest
  make library
  cd $current
  echo "done"
fi

# setup

mkdir -p build
cd build

# clean
rm -rf *

# build
cmake ..
