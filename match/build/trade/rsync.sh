#!/bin/bash

##leave last 2 dump and flow
thispath=`pwd`

ssh -n $1 "mkdir $thispath 1>/dev/null &2>1"
ssh -n $1 "mkdir $thispath/flow 1>/dev/null &2>1"
ssh -n $1 "mkdir $thispath/snap 1>/dev/null &2>1"

rsync -avz --log-file=rsync.log   flow/inputall*.flow        $1:$thispath/flow/
rsync -avz --log-file=rsync.log   snap/snap.*1000000        $1:$thispath/snap/

./backup_del.sh
