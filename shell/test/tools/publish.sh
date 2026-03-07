#!/bin/bash

User=`whoami`

workdir=$(cd $(dirname $0); pwd)

cd $workdir

chmod u+x wsclient
