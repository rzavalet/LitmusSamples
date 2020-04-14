#!/bin/bash

setsched Linux
setsched P-RES

resctl -n 1000 -c 0 -t table-driven -m 200 '[0, 50)' '[100, 150)'
if [ $? -ne 0 ]; then
  echo "Failed to start reservation"
  exit 255
fi

resctl -n 1001 -c 0 -t table-driven -m 200 '[50, 100)' '[150, 200)'
if [ $? -ne 0 ]; then
  echo "Failed to start reservation"
  exit 255
fi

resctl -n 2000 -c 1 -t table-driven -m 200 '[0, 50)' '[100, 150)'
if [ $? -ne 0 ]; then
  echo "Failed to start reservation"
  exit 255
fi

resctl -n 2001 -c 1 -t table-driven -m 200 '[50, 100)' '[150, 200)'
if [ $? -ne 0 ]; then
  echo "Failed to start reservation"
  exit 255
fi

sleep 5;

../Source/reservation &

sleep 5

release_ts
if [ $? -ne 0 ]; then
  echo "Failed to release tasks"
  exit 255
fi


wait
