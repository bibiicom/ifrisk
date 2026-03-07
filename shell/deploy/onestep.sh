#!/bin/bash
if [ ! "$1" == "Y" ]
then
	./get.sh Y
	stop.sh Y $1 $2
	clean.sh $1 $2
	./publish.sh Y $1 $2
	start.sh $1 $2
	show.sh
else
	stop.sh Y $2 $3
	clean.sh $2 $3
	./publish.sh Y $2 $3
	start.sh $2 $3
	show.sh
fi

