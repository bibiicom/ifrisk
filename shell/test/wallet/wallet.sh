#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd "$workdir" || exit 1

while true; do
  for project in arb; do
    cd "$project" || continue
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] 运行 deposit" >> deposit.log
    ./deposit >> deposit.log
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] 运行 withdraw" >> withdraw.log
    ./withdraw >> withdraw.log
    cd ..
  done
  sleep 1
done
