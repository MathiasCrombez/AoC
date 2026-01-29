use regex::Regex;
use crate::helpers::Part;

pub const RESULTS: [[i64; 2]; 2] = [
    [1227775554, 4174379265],    // Expected results for example input: Part1, Part2
    [24157613387, 33832678380], // Expected results for actual data input: Part1, Part2
];

pub fn parse_input(input: &str, mode: &Part) -> i64 {

    let re = Regex::new(r"(\d+)-(\d+)").unwrap();
    let mut result: i64 = 0;

    for (_, [start_str, end_str]) in re.captures_iter(input).filter_map(|cap| {
        Some((cap.get(0).unwrap().as_str(), [cap.get(1).unwrap().as_str(), cap.get(2).unwrap().as_str()]))
    }) {
        let start: i64 = start_str.parse().unwrap();
        let end: i64 = end_str.parse().unwrap();

        for num in start..=end {
            let num_str = num.to_string();
            let len = num_str.len();
            match mode {
                Part::Part1 => {
                    if len % 2 != 0 {
                        continue; // Skip numbers with odd length
                    }
                    let mid = len / 2;
                    let first_half = &num_str[..mid];
                    let second_half = &num_str[mid..];
                    if first_half == second_half {
                        result += num;
                    }
                }
                Part::Part2 => {
                    for i in 1..=len/2 {
                        if (len % i) != 0 {
                            continue;
                        }
                        // Check if the number consists of repeating segments
                        let segment_bytes = &num_str.as_bytes()[..i];
                        // Convert the string in bytes slice for comparison and check all slices
                        let all_match = num_str.as_bytes()
                            .chunks_exact(i)
                            .all(|chunk| chunk == segment_bytes);
                        
                        if all_match {
                            result += num;
                            break;
                        }
                    }
                }
            }
        }
    }

    result
}