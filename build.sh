#!/bin/bash

# this script compiles Qt for development use

if [ -n "$1" ]
then
	DEST_DIR=$1
else
	DEST_DIR=/opt/qt
fi


mkdir -p "$DEST_DIR"

if [ $? -ne 0 ];
then
	echo "Cannot create $DEST_DIR"
	exit $?;
fi

touch ${DEST_DIR}/.foo && rm -f ${DEST_DIR}/.foo

if [ $? -ne 0 ];
then
	echo "$DEST_DIR is not writable";
	exit $?;
fi

./configure -opensource -cocoa -no-framework -arch "ppc x86 x86_64" -sdk /Developer/SDKs/MacOSX10.5.sdk -debug-and-release -prefix $DEST_DIR -qt-sql-odbc -qt-sql-sqlite -buildkey Cedrus-Qt-4.6.2 -no-sql-mysql

MAKE_FLAGS="-j`sysctl hw.ncpu | awk '// {print $2*1.5}'`"

make $MAKE_FLAGS

make install