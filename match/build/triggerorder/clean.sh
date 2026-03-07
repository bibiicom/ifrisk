#!/bin/bash

rm -rf *.log
rm -rf dump/*
rm -rf nohup.out
rm -rf *.con


if [ -d "flow" ]
then
        rm -rf flow/*
fi

