#! /usr/bin/env bash
n_threads=${1:-1}
mkdir -p output/
time ./build/asset_conv -t $n_threads -i ./test.csv 2> err.log
echo "Done with $n_threads threads"
cat err.log
