use crate::helpers::Part;

pub const RESULTS: [[i64; 2]; 2] = [
    [3, 6],    // Expected results for example input: Part1, Part2
    [1023, 5899], // Expected results for actual data input: Part1, Part2
];

pub fn parse_input(input: &str, mode: &Part) -> i64 {

    let mut index = 50;
    let mut result: i64 = 0;

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
        
        let value: i64 = line[1..].parse().unwrap_or(0);
        let offset: i64 = value / 100;
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
    result
}