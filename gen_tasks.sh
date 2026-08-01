#! /usr/bin/env bash
n_threads=${1:-1}
cd build/
mkdir -p output/
time ../scripts/gen_tasks.py ../data ./output/ 480 | ./asset_conv -t $n_threads 2> err.log
echo "Done with $n_threads threads"
cat err.log
