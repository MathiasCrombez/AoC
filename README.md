# Advent of Code 2025 Solutions

## How to Run
for i in $(seq 1 9) $(seq 11 12) ; do echo "# AoC 2025 Day $i" ; gcc -o ./aoc$i.out ./aoc$i.c -lm && ./aoc$i.out || break ; done