workdir=$(cd $(dirname $0); pwd)
cd $workdir
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

echo "Net Port Status is:"
echo ------------------------------------------
for module in `ls -d */ | grep -w "$1$2" | awk -F / '{print $1}'`
do
	cd $module
	./chnet.sh
	cd ..
done
echo ------------------------------------------
