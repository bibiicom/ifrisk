#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
ulimit -c unlimited
ulimit  unlimited
umask 0027

workdir=$(cd $(dirname $0); pwd)
cd $workdir
#in case of reading while trade is writing,ignore the max snap
maxsnapredo=`ls -d snap/snap.*.redo 2>/dev/null | awk -F . '{print $2}' | sort -r -n | head -n 1`
if [ "$maxsnapredo" == "0" -o "$maxsnapredo" == "" ]
then
	echo no maxsnapredo
	exit
fi
echo maxsnapredo is $maxsnapredo
maxsnap=0
for presnap in `ls -d snap/snap.* 2>/dev/null | grep -v redo | awk -F . '{print $2}' | sort -r -n`
do
	if [ $presnap -le $maxsnapredo ]
	then
		maxsnap=$presnap
		break
	fi
done
echo maxsnap is $maxsnap

minsnap=`ls -d snap/diff.snap.* 2>/dev/null | awk -F . '{print $3}' | sort -r -n | head -n 1`
if [ "$minsnap" == "" ]
then
	minsnap=0
fi
echo minsnap is $minsnap

if [ -e $0.lock ]
then
	echo $0 is running,exit!
	exit
fi
touch $0.lock

cd snap
for snap in `ls -d snap.* 2>/dev/null`
do
	if [ "${snap##*.}" == "redo" ]
	then
		continue
	fi
	snapnum1=${snap#snap.}
	snapnum=${snapnum1%%.*}
	if [ $snapnum -ge $maxsnap ]
	then
		echo skip $snapnum: maxsnap=$maxsnap
		continue
	fi
	if [ -f diff.$snap ]
	then
		echo skip $snapnum: diff.$snap exist
		continue
	fi
	if [ $snapnum -le $minsnap ]
	then
		echo skip $snapnum: smaller than $minsnap
		continue
	fi
	if [ -d $snap.redo ]
	then
		echo diff $snap ==========
		cd $snap
		for csv in *.csv
		do
			#echo $csv
			sort $csv > 1
			sort ../$snap.redo/$csv > 2
			diff 1 2 > diff.tmp
			if [ -s diff.tmp ]
			then
				echo $csv >> ../diff.$snap		
				head -n 1 $csv >> ../diff.$snap
			fi
			cat diff.tmp >> ../diff.$snap
			rm -rf diff.tmp
			rm -rf 1 2
		done
		cd ..
		
		for inc in `ls -d inc.*.redo 2>/dev/null | awk -F . '{print $2}' | sort -n`
		do
			if [ $inc -eq $snapnum ]
			then
				incx=$inc 
				for csv in inc.$inc.redo/*.csv 
				do
					cat $csv > $csv.merge	
				done
			fi
			if [ $inc -gt $snapnum ]
			then
				if [ -d snap.$inc ]
				then
					break
				fi
				for csv in inc.$inc.redo/*.csv 
				do
					eachcsv=${csv##*.redo/}
					#echo $csv
					#echo $eachcsv
					#echo inc.$incx.redo
					sed 1d $csv >> inc.$incx.redo/$eachcsv.merge
				done
			fi
		done
		for csv in inc.$snapnum/*.csv
		do
			eachcsv=${csv##*/}
			#echo diff $csv inc.$snapnum.redo/$eachcsv.merge
			diff $csv inc.$snapnum.redo/$eachcsv.merge >> diff.$snap
		done
	fi
done
cd ..

rm -rf $0.lock
