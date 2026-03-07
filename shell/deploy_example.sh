#!/bin/bash

build_ip=172.19.1.12
build_id=root

ssh -n $build_id@$build_ip ". .bashrc;cd build;./abuild_trade.sh"

cd control
./deploy_sync
./deploy_last
./deploy_onestep Y
