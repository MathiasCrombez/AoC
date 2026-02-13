use crate::helpers::Part;

pub const RESULTS: [[i64; 2]; 2] = [
    [1227775554, 4174379265],    // Expected results for example input: Part1, Part2
    [24157613387, 33832678380], // Expected results for actual data input: Part1, Part2
];

/*
 * TODO: We could get all numbers with repeating segments and then filter them by the input ranges,
 * instead of checking each number in the range for repeating segments. This would be more
 * efficient, especially for larger ranges.
 */
pub fn parse_input(input: &str, mode: &Part) -> i64 {
    let mut result: i64 = 0;

    for range in input.split(',').map(|s| s.trim()) {
        let (start, end) = range
            .split_once('-')
            .map(|(a, b)| (a.parse().unwrap(), b.parse().unwrap())).unwrap_or((0, 0));

        for num in start..=end {
            let num_str = num.to_string();
            let num_bytes = num_str.as_bytes();
            let len = num_bytes.len();
            match mode {
                Part::Part1 => {
                    if len % 2 != 0 {
                        continue; // Skip numbers with odd length
                    }
                    let mid = len / 2;
                    let first_half = &num_bytes[..mid];
                    let second_half = &num_bytes[mid..];
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
                        let segment_bytes = &num_bytes[..i];
                        // Convert the string in bytes slice for comparison and check all slices
                        let all_match = num_bytes
                            .chunks(i)
                            .skip(1)
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