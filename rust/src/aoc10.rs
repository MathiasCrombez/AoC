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
        let leds : Vec<u8> = line.chars().filter_map(|c| if c == '.' { Some(0) } else if c == '#' { Some(1) } else { None }).collect();

        let re = Regex::new(r"\(((\d+,)*\d+)\)").unwrap();

        let buttons = re.captures_iter(line)
            .filter_map(|cap| {
                let nums: Vec<u8> = cap[1].split(',').filter_map(|s| s.trim().parse::<u8>().ok()).collect();
                Some(nums)
            })
            .collect::<Vec<Vec<u8>>>();


        let max_combinations: usize = 1 << buttons.len();

        for combination in 0..max_combinations {
            let mut leds = leds.clone();
            for b in 0..buttons.len() {
                if (combination & (1 << b)) != 0 {
                    for &led_idx in &buttons[b] {
                        // Toggle the LED at led_idx
                        if let Some(led) = leds.get_mut(led_idx as usize) {
                            *led ^= 1; // Toggle between 0 and 1
                        }
                    }
                }
            }
            let matches = leds.iter().filter(|&&led| led == 0).count() == leds.len();

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
