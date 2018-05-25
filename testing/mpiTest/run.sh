#!/bin/bash

mpirun --tune params.conf -n $1 -pernode -hostfile workingHosts testMpi
