use crate::helpers::Part;

pub const RESULTS: [[i64; 2]; 2] = [
    [357, 3121910778619],    // Expected results for example input: Part1, Part2
    [16993, 168617068915447], // Expected results for actual data input: Part1, Part2
];

pub fn parse_input(input: &str, mode: &Part) -> i64 {

    let mut result: i64 = 0;

    for num_str in input.lines().map(|line| line.trim()).filter(|line| !line.is_empty()) {
        let num_slice = num_str.as_bytes();
        let mut max: [u8; 12] = [0; 12];
        let joltage_digits = match mode {
            Part::Part1 => 2,
            Part::Part2 => 12,
        };
        
        for idx in 0..num_slice.len() {
            let digit_value = num_slice[idx] - b'0';
            let remaining = num_slice.len() - idx - 1;
            
            match mode {
                Part::Part1 => {
                    if digit_value > max[0] && remaining >= joltage_digits - 1 {
                        max[0] = digit_value;
                        max[1] = 0;
                    } else if digit_value > max[1] {
                        max[1] = digit_value;
                    }
                }
                Part::Part2 => {
                    let mut inserted = false;
                    for i in 0..joltage_digits {
                        if digit_value > max[i] && remaining >= joltage_digits - 1 - i {
                            max[i] = digit_value;
                            max[i + 1..].fill(0);
                            inserted = true;
                            break;
                        }
                    }
                    if !inserted {
                        max[joltage_digits - 1] = max[joltage_digits - 1].max(digit_value);
                    }
                }
            }
        }
        let mut value: i64 = 0;
        for i in 0..joltage_digits {
            value = value * 10 + (max[i] as i64);
        }
        result += value;
    }

    result
}