#!/bin/bash
srcversion=`cat version`
tarname=$srcversion.tar
tar cvf  $tarname "../$(basename "$PWD")/build" 1>/dev/null
gzip $tarname
