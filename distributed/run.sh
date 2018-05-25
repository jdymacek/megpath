#!/bin/bash

mpirun --tune params.conf -n $3 -pernode -hostfile workingHosts distnmf $1 $2
