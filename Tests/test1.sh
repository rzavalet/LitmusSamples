#!/bin/bash

#-------------------------------------
# Start rtspin with:
# Sync release,
# Using verbose
# Attached to processor 1
#-------------------------------------
rtspin -w -v -p 0 -d 20 10 100 5 &
rtspin -w -v -p 1 -d 20 10 100 5 &

rtspin -w -v -p 0 -d 20 10 100 5 &
rtspin -w -v -p 1 -d 20 10 100 5 &

rtspin -w -v -p 0 -d 20 10 100 5 &
rtspin -w -v -p 1 -d 20 10 100 5 &

sleep 10

release_ts

wait
