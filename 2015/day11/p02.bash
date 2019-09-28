#!/usr/bin/bash

read INPUT

PW_1=$(./p01.py <<< $INPUT)
echo $PW_1

PW_2=$(./p01.py <<< $PW_1)
echo $PW_2
