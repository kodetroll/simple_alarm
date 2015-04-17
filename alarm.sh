#!/bin/bash
# alarm.sh -  A script to use the alarm command
#
# usage:
#
# cmd> 'alarm.sh <TIME>'
#
# Where:
#      <TIME> Is the amount of time, defaults to 30 S if not given
#
# (C) 2015 KB4OID Labs, a division of Kodetroll Heavy Industries
# Project: simple_alarm
# Author: Kodetroll (SLM)
# Date: April 17 2015
#
# Ver: 1.0
#
TIME=30
if [ ! -z "$1" ]; then
    TIME=$1
fi
alarm --verbose -i 10 -t $TIME -a PLAY_ERRR
