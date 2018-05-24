#!/bin/bash

mpirun -n $1 -pernode -hostfile workingHosts testMpi
