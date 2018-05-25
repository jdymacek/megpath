#!/bin/bash
mpirun --tune params.conf  -np $1 -pernode --hostfile workingHosts testMpi
