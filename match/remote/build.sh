#!/bin/bash

if [ "$1" == "" ]; then
	echo "build.sh projectname [branch] [version]"
	exit
else
	project_name=$1
fi


git_ip=`more $1.ini | grep git_ip | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
echo git_ip:$git_ip
git_path=`more $1.ini | grep git_path | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
echo git_path:$git_path

work_dir=`pwd`
release_dir=$work_dir/release/$project_name
echo workdir: $workdir
echo mkdir -p $release_dir
mkdir -p $release_dir

rm -rf $work_dir/$project_name 

git config --global credential.helper store
mkdir $work_dir/clone

echo $work_dir/clone/$project_name
if [ ! -d "$work_dir/clone/$project_name" ]
then
	echo git clone https://$git_ip/$git_path.git $work_dir/clone/$project_name
	git clone https://$git_ip/$git_path.git $work_dir/clone/$project_name
else
	cd $work_dir/clone/$project_name
	git pull
	cd -
fi

srcversion=`cat $work_dir/clone/$project_name/version`
cd $work_dir/clone/$project_name

if [ "$2" == "" ]; then
	svnversion=`git log -1 --format=%ct | xargs -I{} date -d @{} +%Y%m%d.%H%M`
	buildversion=${srcversion}_$svnversion
	git archive main | gzip > "$release_dir/$buildversion.src.tar.gz"
else
	git switch $2
	svnversion=`git log -1 --format=%ct | xargs -I{} date -d @{} +%Y%m%d.%H%M`
	buildversion=${srcversion}_$2_$svnversion
	git archive $2 | gzip > "$release_dir/$buildversion.src.tar.gz"
fi

echo version:[$buildversion]

cd $work_dir/
mkdir $project_name
tar -xvf $release_dir/$buildversion.src.tar.gz -C $project_name

cd $work_dir/$project_name
echo $buildversion > version
chmod u+x *.sh
./build.sh
./pack.sh

cd $work_dir
release_user=`more $1.ini | grep release_user | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
echo release_user:$release_user
if [ "$release_user" == "" ]; then
	echo "to 127.0.0.1"
	release_ip=127.0.0.1
	cp -f $work_dir/$project_name/*.tar.gz $release_dir/
else
	release_path=`more $1.ini | grep release_path | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
	echo release_path:$release_path
	release_ip=`more $1.ini | grep release_ip | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
	if [ "$release_ip" == "" ]; then
		release_ip=127.0.0.1
	fi
	echo release_ip:$release_ip

	ssh -n $release_user@$release_ip mkdir -p $release_path/$project_name
	scp $work_dir/$project_name/*.tar.gz $release_user@$release_ip:$release_path/$project_name/
fi




