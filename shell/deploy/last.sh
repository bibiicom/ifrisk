#!/bin/bash

if [ "$1" == "" ]
then
	project=match
else
	project=$1
fi

release=`ls -t ../release/$project/*.tar.gz | head -n 1`
tar -xvf $release -C ../test/
