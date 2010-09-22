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

./configure -opensource -framework -universal -arch x86 -arch ppc -arch x86_64 -sdk /Developer/SDKs/MacOSX10.5.sdk -debug-and-release -prefix $DEST_DIR -qt-sql-odbc -qt-sql-sqlite -buildkey Cedrus-Qt-4.7.0 -no-sql-mysql -no-sql-odbc -no-qt3support -nomake demos -nomake examples

MAKE_FLAGS="-j`sysctl hw.ncpu | awk '// {print $2*1.5}'`"

make $MAKE_FLAGS

make install

defaults write ~/.MacOSX/environment QTDIR "$DEST_DIR"

echo "---------------------------------------------------"
echo "If you do not already have a QTDIR environment variable, you will need to"
echo "log out of Mac OS X and log back in again.  This variable has been set"
echo "in ~/.MacOSX/environment.plist"
echo "---------------------------------------------------"

