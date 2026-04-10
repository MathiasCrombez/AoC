use crate::helpers::Part;
use regex::Regex;

pub const RESULTS: [[i64; 2]; 2] = [
    [7, 33],    // Expected results for example input: Part1, Part2
    [479, 7264308110], // Expected results for actual data input: Part1, Part2
];


pub fn parse_input(input: &str, mode: &Part) -> i64 {

    let mut result: i64 = 0;

    for line in input.lines().filter(|l| !l.is_empty()) {
        let mut line_result = 0;
        let leds : u32 = line.split_ascii_whitespace().nth(0).unwrap()[1..].bytes().enumerate().fold(0, |acc, (i, b)| {
            if b == b'#' {
                acc | (1 << i)
            } else {
                acc
            }
        });

        let re = Regex::new(r"\(((\d+,)*\d+)\)").unwrap();

        let buttons = re.captures_iter(line)
            .filter_map(|cap| {
                let action: u32 = cap[1].split(',').filter_map(|s| s.trim().parse::<u8>().ok()).fold(0, |acc, n| acc | (1 << n));
                Some(action)
            })
            .collect::<Vec<u32>>();


        let max_combinations: usize = 1 << buttons.len();
        let mut combinations : Vec<u32> = Vec::with_capacity(max_combinations);
        for i in 0..buttons.len() {
            combinations.push(1 << i);
        }
        for i in 0..buttons.len() {
            for j in i+1..buttons.len() {
                combinations.push(1 << i | 1 << j);
            }
        }

        for combination in 0..max_combinations {
            let mut result_leds = 0;
            for b in 0..buttons.len() {
                if (combination & (1 << b)) != 0 {
                    // Toggle the LEDs for the current button
                    result_leds ^= buttons[b];
                }
            }
            let matches = result_leds == leds;

            if matches {
                let num_buttons_pressed = combination.count_ones() as i64;
                if num_buttons_pressed < line_result || line_result == 0 {
                    line_result = num_buttons_pressed;
                }
                
            }
        }
        result += line_result;
    }

    result
}
