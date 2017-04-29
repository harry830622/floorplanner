#!/usr/bin/env bash

exe="$1"
alpha="$2"

for f in ./input/*.block; do
  f_base=$(basename "$f" ".block")
  run="$exe $alpha $f ./input/$f_base.nets ./output/$f_base.rpt"
  echo "$run"
  $run
done
