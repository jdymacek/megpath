#!/bin/bash

mpirun --tune params.conf -n $2 -pernode -hostfile workingHosts distnmf $1 $3 $4 $5 $6
