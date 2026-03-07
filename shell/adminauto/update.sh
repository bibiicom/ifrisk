#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

svn cleanup
svn update
chmod u+x *.sh
chmod u+x */*.sh