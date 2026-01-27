use std::fs;

const INPUT_FILES : [&str; 2] = [
    "../../aoc1_ex.txt",
    "../../aoc1_data.txt",
];

enum Part {
    Part1,
    Part2,
}

const RESULTS: [[i32; 2]; 2] = [
    [3, 6],    // Expected results for example input: Part1, Part2
    [1023, 5899], // Expected results for actual data input: Part1, Part2
];

fn parse_input(input: &str, mode: &Part) -> i32 {

    let mut index = 50;
    let mut result = 0;

    for line in input.lines() {
        let line = line.trim();
        if line.is_empty() {
            continue;
        }
        
        let direction = match line.chars().next() {
            Some('R') => true,
            Some('L') => false,
            _ => continue,
        };
        
        let value: i32 = line[1..].parse().unwrap_or(0);
        let offset = value / 100;
        let value_modulo = value % 100;
        
        let new_index = if direction {
            (index + value_modulo) % 100
        } else {
            (index - value_modulo + 100) % 100
        };

        match mode {
            Part::Part1 => {
                if new_index == 0 {
                    result += 1;
                }
            }
            Part::Part2 => {
                let wrapped = if direction {
                    index != 0 && (index + value_modulo) >= 100
                } else {
                    index != 0 && (index - value_modulo) <= 0
                };
                if wrapped {
                    result += 1;
                }
                result += offset;
            }
        }
        index = new_index;
    }
    println!("Result: {}", result);
    result
}


fn main() {
    for mode in [Part::Part1, Part::Part2] {
        for file in INPUT_FILES {
            let input = fs::read_to_string(file).expect("Failed to read input file");
            let result = parse_input(&input, &mode);
            assert_eq!(result, RESULTS[INPUT_FILES.iter().position(|&f| f == file).unwrap()][match mode {
               Part::Part1 => 0,
               Part::Part2 => 1,
            }]);
        }
    }
}