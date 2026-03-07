#!/bin/bash

User=`whoami`

app=`ps -fu $User | grep -w "./repair_*.sh" | grep -v grep`
proc=`ps -fu $User | grep -w "./repair_*.sh" | grep -v grep | awk '{print $2}'`
if [ "$proc" == "" ]
then
	printf "repair_*.sh is not running\n"
else
	echo "Stop [ $app ] ..."
	kill -9 $proc
fi

app=`ps -fu $User | grep -w "svn checkout" | grep -v grep`
proc=`ps -fu $User | grep -w "svn checkout" | grep -v grep | awk '{print $2}'`
if [ "$proc" == "" ]
then
	printf "svn checkout is not running\n"
else
	echo "Stop [ $app ] ..."
	kill -9 $proc
fi

app=`ps -fu $User | grep -w "svn update" | grep -v grep`
proc=`ps -fu $User | grep -w "svn update" | grep -v grep | awk '{print $2}'`
if [ "$proc" == "" ]
then
	printf "svn update is not running\n"
else
	echo "Stop [ $app ] ..."
	kill -9 $proc
fi

app=`ps -fu $User | grep -w "svn commit" | grep -v grep`
proc=`ps -fu $User | grep -w "svn commit" | grep -v grep | awk '{print $2}'`
if [ "$proc" == "" ]
then
	printf "svn commit is not running\n"
else
	echo "Stop [ $app ] ..."
	kill -9 $proc
fi

svn cleanup
