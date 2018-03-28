#!/bin/bash

make build

SIZES="1 8 64 1024 8000 32000"

for SIZE in $SIZES; do
    ./run.sh $SIZE
done