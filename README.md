# fixed-outline-floorplanning

## How to build

```sh
make
```

## Usage

```sh
Usage:
  ./fp [--verbose] [--seed <number>] [--sa <classical|fast|both|fast-5>]
  [--draw-all <file>] [--draw-only-best <file>]
  <alpha> <block-input> <net-input> <output>
Options:
  -h [ --help ]          Print help messages
  -v [ --verbose ]       Print detail informations during floorplanning
  --seed NUM             Seed for random number generator
  --sa MODE (=classical) SA mode: classical, fast, both or fast-5
  --draw-all FILE        Draw all floorplans
  --draw-only-best FILE  Draw only best floorplan
  --alpha NUM            Alpha
  --block-input FILE     Block input
  --net-input FILE       Net input
  --output FILE          Output
```

## SA mode
 - classical: Run classical SA until floorplan is feasible. (default)
 - fast: Run fast SA until floorplan is feasible.
 - both: Run classical SA and fast SA until floorplan is feasible, then
   output the better one.
 - fast-5: Run fast SA 5 times and output the best floorplan.
 
## Seed
Since every perturbation in SA is randomly made, we can fix the seed to
get the same sequence of preturbations, thus the same floorplan result
every time.

## GUI

You can run floorplanning on my [web app](https://b-star-tree.yhchang.me).
