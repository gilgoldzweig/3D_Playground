#!/bin/sh
bindir=$(pwd)
cd /Users/gilgoldzweig/projects/ogl/playground/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/gilgoldzweig/projects/ogl/cmake-build-debug/playground 
	else
		"/Users/gilgoldzweig/projects/ogl/cmake-build-debug/playground"  
	fi
else
	"/Users/gilgoldzweig/projects/ogl/cmake-build-debug/playground"  
fi
