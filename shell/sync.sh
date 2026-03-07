#!/bin/bash

sync_ipaddr=`more deploy/env.ini | grep sync_ipaddr | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
sync_userid=`more deploy/env.ini | grep sync_userid | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`

ssh -n $sync_userid@$sync_ipaddr "rm -rf test; ./update.sh"

rsync -avz --delete --exclude ".*" --exclude "release" --exclude "hisdata" --exclude "deploy/list.run" --exclude "*/*.ini" --exclude "deploy/*.pem" --exclude "deploy/*.conf" --exclude "run" --exclude "deploy/network.ini" $sync_userid@$sync_ipaddr:./ ./

chmod u+x control/*
chmod u+x *.sh
chmod u+x */*.sh
chmod u+x */*/*.sh

