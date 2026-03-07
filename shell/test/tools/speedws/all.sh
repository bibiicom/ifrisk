#!/bin/bash
i=0
while [ $i -lt $1 ]
do
        nohup ./start.sh &
        i=`expr $i + 1`
done