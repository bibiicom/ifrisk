#!/bin/bash

User=`whoami`
url=`curl -s ip.sb`
path=`pwd`
echo "scp $User@$url:$path/$1 ./"