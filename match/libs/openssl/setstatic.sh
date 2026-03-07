#!/bin/bash
gccversion=`gcc --version | grep gcc | awk '{ print $3 }'`
rm -rf 1.1.0h/static/linux_x64 1>/dev/null 2>&1
if [ -d 1.1.0h/static/linux_"$gccversion"_x64 ]
then
	cp -r 1.1.0h/static/linux_"$gccversion"_x64 1.1.0h/static/linux_x64
else
	cp -r 1.1.0h/static/linux_x64.gen 1.1.0h/static/linux_x64
fi

rm -rf 1.1.0h/static/windows_x64 1>/dev/null 2>&1
cp -r 1.1.0h/static/windows_x64.gen 1.1.0h/static/windows_x64
