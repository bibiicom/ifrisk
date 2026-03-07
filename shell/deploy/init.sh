#!/bin/bash

if [ "$1" == "" ]
then
	echo "usage: init.sh parma [module]"
	echo "Options:"
	echo " param	:[8g|16g|32g|64g] if 8g use "../test/*/*.8g" as config file"
	echo " module	:[trade|trade1|trade2|query1|query|client...] if trade init only trade"
	exit
fi

chmod u+x *.sh

if [ -f list.run.$1 ]
then
	#echo cp list.run.$1 list.run
	cp -v list.run.$1 list.run
fi

if [ ! -f network.ini ]
then
	echo no network.ini
fi

mkdir ../run 2>/dev/null
for ini in `ls ../test/*.$1 2>/dev/null`
do
	realini=${ini//.$1/}
	cp -v $ini $realini
done

cp -v ../test/*.sh ../run/
#cp -v ../test/*.sql ../run/
#cp -rfv test/*.* ../run/

cp network.ini ../admin/
cp network.ini ../adminauto/
cp network.ini ../history/

chmod u+x ../run/*.sh

cntexpr=`wc -l list.run|awk '{print $1}'`
cnt=`expr $cntexpr`

i=0
while [ $i -lt $cnt ]
do
	i=`expr $i + 1 `
	read srv no
	appname=$srv
	if [ "$no" == "" ]
	then
		module=$srv
	else
		if [ "$no" -ge 0 ] 2>/dev/null 
		then
			module=`echo $srv$no`
		else
			module=`echo ${srv}_$no`
		fi
	fi
	if [ ! "$2" == "" ]
	then
		if [ ! "$2" == "$appname" ]
		then
			continue
		fi
	fi

	printf "init %-14s -------------------------------------------------------\n" $module 
	
	mkdir ../run/$module 2>/dev/null
	cp -v network.ini ../run/$module/

	if [ -f ../test/$appname/init.sh ]
	then 
		chmod u+x ../test/$appname/init.sh 
		thispath=`pwd`
		../test/$appname/init.sh ../../run/$module/ $thispath
	else
		for ini in `ls ../test/$appname/*.$1 2>/dev/null`
		do
			realini=${ini//.$1/}
			cp -v $ini $realini
		done
	
		for ini in ini conf linux sh xml yaml cfg
		do
			#ls ../test/$appname/*.$ini 2>/dev/null
			cp -v ../test/$appname/*.$ini ../run/$module/ 2>/dev/null
			cp -v ../test/$appname/$module.$ini ../run/$module/$appname.$ini 2>/dev/null
			if [ -d "test/$appname" ]
			then
				cp -v test/$appname/*.$ini ../run/$module/ 2>/dev/null
			fi
			if [ -d "test/$module" ]
			then
				cp -v test/$module/*.$ini ../run/$module/ 2>/dev/null
			fi
		done
		#echo chmod to $module/*.sh
		chmod u+x ../run/$module/*.sh
	fi
done < list.run
