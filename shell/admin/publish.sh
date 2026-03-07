#!/bin/bash

workdir=$(cd $(dirname $0); pwd)

cd $workdir

cp *.sh $1/
cp *.example $1/
cp *.sql $1/


