@echo off
@setlocal enabledelayedexpansion

set remote=43.134.74.19

scp *.cpp *.h root@%remote%:./match/build/apilinks/OneMegaToStable/

ssh -n trade@%remote% ". .bashrc;stop.sh marketmaker Y"

ssh -n root@%remote% "cd ./match/build/apilinks/OneMegaToStable/;rm -rf *.so;make -f makefile.linux;scp *.so trade@127.0.0.1:./run/marketmaker/"

ssh -n trade@%remote% ". .bashrc;start.sh marketmaker"

pause