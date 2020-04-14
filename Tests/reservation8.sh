#!/bin/bash

CURRES=P-RES
setsched Linux
setsched ${CURRES}

echo "Creating reservation..."
resctl -n 1000 -c 0 -t polling-sporadic -b 30 -p 100
resctl -n 1001 -c 0 -t polling-sporadic -b 20 -p 50

echo "Creating tasks..."
rtspin -v -w -p 0 -r 1000 10 100 5 &

rtspin -v -w -p 0 -r 1001 10 50 5 &
rtspin -v -w -p 0 -r 1001 10 50 5 &

sleep 5

release_ts

echo "Waiting for tasks to finish..."
wait
