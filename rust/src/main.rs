
use std::fs;
use colored::Colorize;
mod helpers;
use helpers::Part;
mod aoc1;
mod aoc2;
mod aoc3;
mod aoc4;
mod aoc5;
mod aoc6;
mod aoc7;
mod aoc8;
mod aoc10;


struct Day {
    name: &'static str, results: [[i64; 2]; 2], parser: fn(&str, &Part) -> i64
}

const DAYS: [Day; 9] = [
    Day {
        name: "aoc1",
        results: aoc1::RESULTS,
        parser: aoc1::parse_input,
    },
    Day {
        name: "aoc2",
        results: aoc2::RESULTS,
        parser: aoc2::parse_input,
    },
    Day {
        name: "aoc3",
        results: aoc3::RESULTS,
        parser: aoc3::parse_input,
    },
    Day {
        name: "aoc4",
        results: aoc4::RESULTS,
        parser: aoc4::parse_input,
    },
    Day {
        name: "aoc5",
        results: aoc5::RESULTS,
        parser: aoc5::parse_input,
    },
    Day {
        name: "aoc6",
        results: aoc6::RESULTS,
        parser: aoc6::parse_input,
    },
    Day {
        name: "aoc7",
        results: aoc7::RESULTS,
        parser: aoc7::parse_input,
    },
    Day {
        name: "aoc8",
        results: aoc8::RESULTS,
        parser: aoc8::parse_input,
    },
    Day {
        name: "aoc10",
        results: aoc10::RESULTS,
        parser: aoc10::parse_input,
    },
];



fn main() {
    for day in DAYS.iter() {
        println!("Running tests for {}", day.name);
        let results = day.results;
        for mode in [Part::Part1, Part::Part2] {
            let input_files = [
                format!("../{}_ex.txt", day.name),
                format!("../{}_data.txt", day.name),
            ];
            for file in &input_files {
                let before = std::time::Instant::now();
                let input = fs::read_to_string(&file).expect("Failed to read input file");
                // processing
                let result = (day.parser)(&input, &mode);

                let expected_value = results[input_files.iter().position(|f| f == file).unwrap()][match mode {
                    Part::Part1 => 0,
                    Part::Part2 => 1,
                }];
                
                let mode_name = match mode {
                    Part::Part1 => "Part 1",
                    Part::Part2 => "Part 2",
                };
                let after = std::time::Instant::now();
                let duration = after.duration_since(before);
                if result == expected_value {
                    println!("  {} - {} {}: {} (result: {}) [{:?}]", day.name, file, mode_name, "passed".green(), result, duration);
                } else {
                    println!("  {} - {} {}: {} (expected: {}, got: {}) [{:?}]", day.name, file, mode_name, "failed".red(), expected_value, result, duration);
                }

            }
        }
    }
}
