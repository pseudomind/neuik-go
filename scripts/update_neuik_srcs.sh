#!/usr/bin/env bash
#------------------------------------------------------------------------------#
# This update script is included only to simplify the process of pulling the   #
# current libneuik sources and header files from the upstream repository. This #
# script should never need to be run by a developer using `neuik-go`.          #
#------------------------------------------------------------------------------#

thisDir=`pwd`
dirBase=`basename $thisDir`
if [ ! "$dirBase" = 'scripts' ]; then
	echo 'This script should only be run within the `scripts` directory;'\
		'Aborting.'
	exit
fi

if [ ! "$CLEAN" = '1' ]; then
	echo 'Pulling latest source/header files from upstream NEUIK repository...'
fi

echo 'Removing copies of old copies of C source & header files'
rm -f ../neuik/*.c
rm -f ../neuik/include/*.h

echo 'Deleting the existing subproject folder'
if [ -d 'subproject' ]; then
    rm -rf 'subproject'
fi

if [ "$CLEAN" = '1' ]; then
	exit
fi

echo 'Creating a new subproject folder'
mkdir 'subproject'

echo 'Getting a copy of the latest NEUIK sources'
cd subproject
git clone https://github.com/pseudomind/neuik.git

echo 'Copying over the latest NEUIK C source & header files'
cp neuik/lib/*.c ../../neuik
cp neuik/include/*.h ../../neuik/include

cd ..
if [ -d 'subproject' ]; then
    rm -rf 'subproject'
fi
