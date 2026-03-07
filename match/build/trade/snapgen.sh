#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
ulimit -c unlimited
ulimit  unlimited
umask 0027

workdir=$(cd $(dirname $0); pwd)
cd $workdir

if [ -e $0.lock ]
then
	echo $0 is running,exit!
	exit
fi
touch $0.lock

chmod u+x trade
./trade -r $*

rm -rf $0.lock

