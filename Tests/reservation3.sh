#!/bin/bash

setsched Linux
setsched P-RES

resctl -n 1000 -c 0 -t table-driven -m 200 '[0, 50)' '[100, 150)'
resctl -n 1001 -c 1 -t table-driven -m 200 '[50, 100)' '[150, 200)'

rtspin -w -p 0 -r 1000 -d 40 30 200 5 &
rtspin -w -p 0 -r 1000 -o 100 -d 40 30 200 5 &

rtspin -w -p 1 -r 1001 -o 50 -d 40 30 200 5 &
rtspin -w -p 1 -r 1001 -o 150 -d 40 30 200 5 &

sleep 5

release_ts

wait
