#!/bin/sh
bindir=$(pwd)
cd /Users/gilgoldzweig/projects/3D playground/src/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/gilgoldzweig/projects/3D\ playground/cmake-build-debug/src 
	else
		"/Users/gilgoldzweig/projects/3D\ playground/cmake-build-debug/src"  
	fi
else
	"/Users/gilgoldzweig/projects/3D\ playground/cmake-build-debug/src"  
fi
