#!/bin/sh

# Takes a single library (full path) as an argument and fixes Lightpad's linkage to it (as well as copying it into the bunndle)

lib=$1
base=$(basename $lib)
bundle=Debug/lightpad.app/Contents/MacOS
cp $lib $bundle/$base
chmod 700 $bundle/$base
install_name_tool -change $lib @executable_path/$base $bundle/libNightwaveCore.dylib
