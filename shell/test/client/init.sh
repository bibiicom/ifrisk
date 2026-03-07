#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

# 准备githubpages工程目录
# echo 'init githubpages...'
# if [ ! -d  $workdir/tobetrader.github.io ]
# then
# 	git clone git@github.com:tobetrader/tobetrader.github.io.git
# fi

# cd $workdir/tobetrader.github.io
# git checkout main
# git pull

# 解压nginx
cd $workdir
echo 'init local env...'
tar -zxvf nginx.tar.gz
mkdir $1/nginx
mkdir $1/nginx/conf

rm -rf $1/nginx/conf/nginx.conf
cp -r ./nginx/conf/* $1/nginx/conf/
echo $2/nginx.conf $1/nginx/conf/
cp -r $2/nginx.conf $1/nginx/conf/
chmod u-w $1/nginx/conf/nginx.conf

mkdir $1/nginx/logs
mkdir $1/nginx/html

if [ ! -d "$1/nginx/sbin/" ]; then
	cp -r ./nginx/sbin $1/nginx/
fi

cp *.sh $1/
rm -rf $1/init.sh
rm -rf $1/publish.sh
chmod u+x $1/*.sh


