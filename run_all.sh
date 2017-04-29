#!/usr/bin/env bash

num_of_times="$1"
exe="$2"
alpha="$3"
flags=$(echo $@ | cut -d" " -f 4-)

for i in $(seq 1 $num_of_times); do
  echo "$i th iteration"
  for f in ./input/*.block; do
    f_base=$(basename "$f" ".block")
    run="$exe $alpha $f ./input/$f_base.nets ./output/$f_base.rpt $flags"
    echo "$run"
    $run
  done
done
