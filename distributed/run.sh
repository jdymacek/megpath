#!/bin/bash

mpirun -n $3 -pernode -hostfile workingHosts $1 $2
