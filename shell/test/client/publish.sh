#!/bin/bash

workdir=$(cd $(dirname $0); pwd)

#User=`whoami`
#proc=`ps -fu $User | grep "/client/nginx" | grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
#if [ ! "$proc" == "" ]
#then
#    printf "%-14s is running\n" nginx
#    exit
#fi

cd $workdir

if [ -d build ]; then
	rm -rf $1/nginx/html
	mkdir $1/nginx/html
	echo cp -r build/* $1/nginx/html/
	cp -r build/* $1/nginx/html/
fi

rm -rf $1/nginx/preview 1>/dev/null 2>/dev/null
echo cp -r ../api/i5ting/preview $1/nginx/
cp -r ../api/i5ting/preview $1/nginx/

cp -v ../api/i5ting/example/* $1/nginx/preview/
#cp -v ../api/*.pdf $1/nginx/preview/

# 发布到githubpages
# rm -rf ./tobetrader.github.io/docs

# cp -r ./build ./tobetrader.github.io
# rm -rf ./tobetrader.github.io/docs
# mv ./tobetrader.github.io/build ./tobetrader.github.io/docs

# cd ./tobetrader.github.io
# git config --global user.name $User
# git config --global user.email $User@mail.com

# git add --all .
# git commit -m 'commit'
# git push

