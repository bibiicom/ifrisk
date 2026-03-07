#!/bin/bash

User=`whoami`
ps -fu $User|grep -w "./run.sh" | grep -v grep
ps -fu $User|grep -w "curl" | grep -v grep
