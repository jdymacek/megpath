#!/bin/bash

declare -a arr=("pp")

i=1
#how many data points to collect
while [ $i -le $1 ]
do
	./getData.sh
	j=1

	#how many needed to average
	while [ $j -le $2 ]
	do

		#for every algorithm
		for k in "${arr[@]}"
		do
			make
			./run.sh args "$k" 20
		done

	#average them here after collecting
	j=$(( $j + 1 ))
	done

i=$(( $i + 1 ))
done
