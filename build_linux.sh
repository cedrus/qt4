#!/bin/bash

# this script compiles Qt for development use

if [ -n "$1" ]
then
	DEST_DIR=$1
else
	DEST_DIR=/usr/local
fi

echo "We may need to get some build dependencies from the debian archive."
echo "The following commands requires sudo access to install the necessary files"
sudo apt-get build-dep qt4-x11
sudo apt-get install libphonon-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev

./configure -opensource -debug-and-release -prefix $DEST_DIR -buildkey Cedrus-Qt-4.7.0 -nomake demos -nomake examples -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/libxml2

MAKE_FLAGS="-j3"

make $MAKE_FLAGS

sudo make install

