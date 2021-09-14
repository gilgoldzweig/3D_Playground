#!/bin/sh
bindir=$(pwd)
cd /Users/gilgoldzweig/projects/ogl/tutorial09_vbo_indexing/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/gilgoldzweig/projects/ogl/cmake-build-debug/tutorial09_AssImp 
	else
		"/Users/gilgoldzweig/projects/ogl/cmake-build-debug/tutorial09_AssImp"  
	fi
else
	"/Users/gilgoldzweig/projects/ogl/cmake-build-debug/tutorial09_AssImp"  
fi
