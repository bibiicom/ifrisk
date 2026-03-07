#!/bin/bash

maxsnap=`ls -d run/trade/snap/snap.* 2>/dev/null | awk -F . '{print $2}' | sort -r -n | head -n 1`
if [ ! "$maxsnap" == "" ]
then
	rm -rf snap.0
	if [ -d run/trade/snap/snap.$maxsnap ]
	then
		cp -rv run/trade/snap/snap.$maxsnap snap.0
	else
		cp -rv run/trade/snap/snap.$maxsnap.* snap.0
	fi
fi

thishostname=`hostname`
#rm -rf env_$thishostname.tar.gz
thistime=`date +%Y%m%d%H%M`

cd control
./list_get
./crontab_get
cd ..

tar cvf env_$thishostname.$thistime.tar control/list.hosts control/*.list.run control/*.sh deploy/list.run deploy/*.ini deploy/test* crontab_* snap.0

if [ -f deploy/*.conf ]
then
	tar -rvf env_$thishostname.$thistime.tar deploy/*.conf
fi 

gzip env_$thishostname.$thistime.tar
#sz env_$thishostname.$thistime.tar.gz

