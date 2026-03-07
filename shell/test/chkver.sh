workdir=$(cd $(dirname $0); pwd)
cd $workdir
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

echo "version is:"
echo =================================================================================
if [ "$2" == "" ]
then
	grepname=$1
else
	if [ "$2" -ge 0 ] 2>/dev/null 
	then
		grepname=$1$2
	else
		grepname=`echo ${1}_$2`
	fi
fi

for module in `ls -F | grep "/$" | grep "$grepname" | awk -F / '{print $1}'`
do
	cd $module
	printf "%-14s -----------------------------------------------\n" $module 
	for run in * */sbin/*
	do
		binfile=`file $run | grep executable | grep -v "\.exe" | grep -v script`
		if [ ! "$binfile" == "" ] 
		then
			chmod u+x $run
			./$run -v
		fi
	done
	cd ..
done
echo =================================================================================
