#!/bin/bash

find . -maxdepth 1 -name "*" -type f -size 0c | grep -v snap | grep -v lock | xargs -n 1 rm -rf
