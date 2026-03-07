#!/bin/bash

echo repair_run.sh [svn_url]

project=cwjjcom

thispath=`pwd`
echo this path is: $thispath
read -r -p "Are You Sure? [Y/n] " input

case $input in
    [yY][eE][sS]|[yY])
		echo "Yes"
		;;

    [nN][oO]|[nN])
		echo "No"
		exit 1
       	;;

    *)
		echo "Invalid input..."
		exit 1
		;;
esac

mkdir repair_tmp 2>/dev/null
cp -vf control/list.hosts repair_tmp/
cp -vf control/*.list.run repair_tmp/
cp -vf control/*.sh repair_tmp/

cp -vf deploy/list.run repair_tmp/
cp -vf deploy/*.ini repair_tmp/
cp -vf deploy/*.conf repair_tmp/
cp -vf deploy/*.pem repair_tmp/
cp -rvf deploy/test* repair_tmp/
cp -vf deploy.sh repair_tmp/

svnpath=`svn info | grep "URL: svn://"`
svnpath=${svnpath#URL:}

rm -rf .svn
rm -rf control deploy test operation redo *.sh README.md

echo svn checkout $svnpath ./
svn checkout $svnpath ./

cp -vf repair_tmp/list.hosts control/
cp -vf repair_tmp/*.list.run control/
cp -vf repair_tmp/*.sh control/

cp -vf repair_tmp/list.run deploy/
cp -vf repair_tmp/*.ini deploy/
cp -vf repair_tmp/*.conf deploy/
cp -rvf repair_tmp/test* deploy/

cp -rvf repair_tmp/deploy.sh ./

chmod u+x control/*
chmod u+x *.sh
