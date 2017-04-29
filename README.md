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

## Web App

You can run floorplanning on my [web app](https://b-star-tree.yhchang.me).
