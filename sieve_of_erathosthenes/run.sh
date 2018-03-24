#!/bin/bash

if [ "$#" -ne 2 ]; then
    exit "Need 2 Arguments. ./batch_sieve <CPUS> <NUMBERS_TO_ITERATE_THROUGH>"
fi

# Arguments
CPUS=$1
N=$2

make build

qsub -q cluster-short -1 nodes=1:ppn="$CPUS" ./batch_sieve.sh -F "$CPUS $N"