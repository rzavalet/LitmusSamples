#!/bin/bash

CURRES=P-RES
setsched Linux
setsched ${CURRES}

echo "Creating reservation..."
resctl -n 1000 -c 0 -t polling-sporadic -b 25 -p 100
resctl -n 1001 -c 0 -t polling-sporadic -b 20 -p 50

echo "Creating tasks..."
dmesg | rtspin -S -w -v -p 0 -r 1000 10 100 5 &
dmesg | rtspin -S -w -v -p 0 -r 1000 10 100 5 &

dmesg | rtspin -S -w -v -p 0 -r 1001 5 50 5 &
dmesg | rtspin -S -w -v -p 0 -r 1001 5 50 5 &

sleep 5;

release_ts

echo "Waiting for tasks to finish..."
wait

