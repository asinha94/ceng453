#!/bin/bash

SIZE="1000000 100000000"

for N in $SIZE; do
    for CPUS in `seq 1 8`; do
        ./run.sh $CPUS $N
    done
done