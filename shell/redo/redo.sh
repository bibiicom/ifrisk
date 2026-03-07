#!/bin/bash
project=trade

if [ "$1" == "-h" ]
then
	echo "usage: redo.sh from(0|202202221000) to(202202231100|max) tradeparam(-r|-c)"
	exit
fi

if [ "$1" == "" ]
then
	from=0
else
	from=$1
fi

if [ "$2" == "max" ]
then
	to=99999999999999
else
	if [ "$2" == "" ]
	then
		to=$from
	else
		to=$2
	fi
fi
echo from[$from]to[$to]

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
ulimit -c unlimited
ulimit  unlimited
umask 0027

workdir=$(cd $(dirname $0); pwd)
cd $workdir
cd $project

cp -rvf ../../run/$project/*.ini ./

for backuptar in ../../backup/*.tar.gz
do
	backupnum1=${backuptar##*/}
	backupnum=${backupnum1%*.tar.gz}
	if [ $backupnum -ge $from ]&&[ $backupnum -le $to ]
	then
		echo $backupnum
		rm -rf $workdir/$backupnum.diff
		if [ ! -d ../backup/$backupnum ]
		then
			echo tar xvf $backuptar -C ../backup/
			tar xvf $backuptar -C ../backup/
		fi
		minsnapnum=999999999999999999
		firstsnap=""
		for snap in ../backup/$backupnum/$project/snap/snap.*
		do
			eachsnapnum1=${snap##*snap.}
			eachsnapnum=${eachsnapnum1%%.*}
			if [ $minsnapnum -gt $eachsnapnum ]
			then
				minsnapnum=$eachsnapnum
				firstsnap=$snap
			fi
		done
		rm -rf snap/*
		rm -rf *.log
		rm -rf dump/*
		rm -rf flow/*
		cp -rv ../backup/$backupnum/$project/flow/inputall.*.flow flow/
		#echo cp -r $firstsnap snap/
		cp -rv $firstsnap snap/
		tradesnap=`ls -d snap/snap.*`
		if [ "$1" == "" ]
		then
			echo ./trade -c
			./trade -c &
		else
			echo ./trade $3
			./trade $3 &
		fi
		wait
		rm -rf $tradesnap
		for diffsnap in snap/snap.*
		do
			diffsnapnum=${diffsnap##snap/}
			touch $workdir/$backupnum.diff
			if [ ! -d ../backup/$backupnum/$project/snap/$diffsnapnum ]
			then
				diffsnapnum=${diffsnapnum%%.redo}
			fi
			if [ -d ../backup/$backupnum/$project/snap/$diffsnapnum ]
			then

				cd $diffsnap
				for csv in *.csv
				do
					sort $csv > 2
					#echo  ../../../backup/$backupnum/$project/snap/$diffsnapnum
					sort ../../../backup/$backupnum/$project/snap/$diffsnapnum/$csv > 1
					diff 1 2 > diff.tmp
					if [ -s diff.tmp ]
					then
						echo $diffsnapnum ========== >> $workdir/$backupnum.diff
						echo $csv ---------------- >> $workdir/$backupnum.diff
						head -n 1 $csv >> $workdir/$backupnum.diff
						cat diff.tmp >> $workdir/$backupnum.diff
					fi
					rm -rf diff.tmp
					rm -rf 1 2
				done
				cd ../../
			fi			
		done
		if [ "$2" == "" ]
		then
			exit
		fi
	fi
done 
cd ..
