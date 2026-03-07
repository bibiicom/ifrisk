#!/bin/bash

rm -rf 202*
cd backup
chmod u+x clean.sh
./clean.sh
cd ..
