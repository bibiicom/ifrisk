#!/bin/bash

cat $1 | grep "us]" | awk -F \{  {'print $1'} | awk -F \[ {'print $5'} | awk {'print $1'} | sort -n
