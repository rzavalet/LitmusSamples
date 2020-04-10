#!/bin/bash

setsched Linux
setsched P-RES

resctl -n 1000 -c 0 -t table-driven -m 200 '[0, 50)' '[100, 150)'
resctl -n 1001 -c 0 -t table-driven -m 200 '[50, 100)' '[150, 200)'
resctl -n 2000 -c 1 -t table-driven -m 200 '[0, 50)' '[100, 150)'
resctl -n 2001 -c 1 -t table-driven -m 200 '[50, 100)' '[150, 200)'
sleep 5;

../Source/reservation &

sleep 5

release_ts

wait
