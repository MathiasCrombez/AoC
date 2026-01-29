# Advent of Code 2025 Solutions

## Introduction

This repository contains my solutions for the Advent of Code 2025 challenges.
The solutions implemented in C are usually self-made, with some slight exceptions noted in the respective files.
The Rust solutions use more heavily AI assistance for code generation, most of the time after doing a first manual conversion C to Rust.
While the C solutions were developed mostly to exercise myself, the Rust solutions were more to explore the Rust language.

## How to Run
for i in $(seq 1 9) $(seq 11 12) ; do echo "# AoC 2025 Day $i" ; gcc -o ./aoc$i.out ./aoc$i.c -lm && ./aoc$i.out || break ; done