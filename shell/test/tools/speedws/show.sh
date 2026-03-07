#!/bin/bash

User=`whoami`
ps -fu $User|grep -w "./wsclient.sh" | grep -v grep
