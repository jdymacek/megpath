#!/bin/bash

mpirun -n $3 -pernode -hostfile workingHosts distnmf $1 $2
