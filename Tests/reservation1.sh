#!/bin/bash

CURRES=`showsched`
setsched Linux
setsched ${CURRES}

resctl -n 1000 -c 0 -t table-driven -m 200 '[0, 50)' '[100, 150)'
resctl -n 1001 -c 1 -t table-driven -m 200 '[0, 50)' '[100, 150)'

yes > /dev/null &
PID1=$!
echo "Pid 1: ${PID1}"

yes > /dev/null &
PID2=$!
echo "Pid 2: ${PID2}"

resctl -a $PID1 -r 1000 -c 0
resctl -a $PID2 -r 1001 -c 1
