#!/bin/bash

if [ "$#" -ne 1 ]; then
    exit "Need 1 Arguments. ./run <SIZE>"
fi

# Arguments
SIZE=$1
qsub -q cluster-short -1 nodes=2:ppn=2 ./batch_pingpong.sh -F "$SIZE"