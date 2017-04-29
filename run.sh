#!/usr/bin/env bash

benchmark="$1"
num_of_times="$2"
exe="$3"
alpha="$4"
flags=$(echo $@ | cut -d" " -f 5-)
seed=$(date +"%s")

cost_sum=0
runtime_sum=0
for i in $(seq 1 $num_of_times); do
  echo "$i th iteration"
  run="$exe $alpha ./input/$benchmark.block ./input/$benchmark.nets ./output/$benchmark.rpt $flags --seed $seed"
  echo "$run"
  cost="$($run | tee /dev/tty | grep "cost" | awk "{print \$5}")"
  runtime="$($run | tee /dev/tty | grep "runtime" | awk "{print \$4}")"
  cost_sum=$(awk "BEGIN {print $cost_sum+$cost; exit}")
  runtime_sum=$(awk "BEGIN {print $runtime_sum+$runtime; exit}")
  seed=$(($seed + 1))
done

average_cost=$(awk "BEGIN {print $cost_sum/$i; exit}")
average_runtime=$(awk "BEGIN {print $runtime_sum/$i; exit}")

echo ""
echo "Average cost: $average_cost"
echo "Average runtime: $average_runtime"
echo ""
