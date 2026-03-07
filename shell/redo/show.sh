#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

User=`whoami`
ps -fu $User | grep -w "./redo.sh"| grep -v grep 
ps -fu $User | grep -w "./trade -r"| grep -v grep
ps -fu $User | grep -w "diff"| grep -v grep

