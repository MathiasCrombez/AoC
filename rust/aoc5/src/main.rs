use std::fs;
use regex::Regex;

const INPUT_FILES : [&str; 2] = [
    "../../aoc5_ex.txt",
    "../../aoc5_data.txt",
];

enum Part {
    Part1,
    Part2,
}

const RESULTS: [[i64; 2]; 2] = [
    [3, 14],    // Expected results for example input: Part1, Part2
    [525, 333892124923577], // Expected results for actual data input: Part1, Part2
];

fn parse_input(input: &str, mode: &Part) -> i64 {

    let mut result: i64 = 0;

    let re = Regex::new(r"(\d+)-(\d+)").unwrap();
    let re_num = Regex::new(r"(?m)^(\d+)$").unwrap();
    let ranges : Vec<(i64, i64)> = 
        re.captures_iter(input)
        .filter_map(|cap| {
         Some((cap.get(0).unwrap().as_str(), [cap.get(1).unwrap().as_str(), cap.get(2).unwrap().as_str()]))
     })
        .map(|(_, [start_str, end_str])| {
            let start: i64 = start_str.parse().unwrap();
            let end: i64 = end_str.parse().unwrap();
            (start, end)
        }).collect();

    if matches!(mode, Part::Part1) {
        let ingredients : Vec<i64> = re_num
            .captures_iter(input)
            .map(|mat| {
                let num_str = mat.get(1).unwrap().as_str();
                num_str.parse::<i64>().unwrap()
            })
            .collect();

        for ingredient in ingredients {
            for &(start, end) in &ranges {
                if ingredient >= start && ingredient <= end {
                    result += 1;
                    break;
                }
            }
        }
    } else {
        let mut ranges = ranges;
        for i in 0..ranges.len() {
            if ranges[i].0 == 0 && ranges[i].1 == 0 {
                continue;
            }
            for j in 0..ranges.len() {
                let range1 = ranges[i];
                let range2 = ranges[j];
                if i == j {
                    continue;
                }
                if ranges[j].0 == 0 && ranges[j].1 == 0 {
                    continue;
                }
                if range1.0 <= range2.0 && range1.1 >= range2.1 {
                    ranges[j] = (0, 0);
                }
                else if range1.0 <= range2.0 && range1.1 >= range2.0 {
                    ranges[i] = (range1.0, range2.1);
                    ranges[j] = (0, 0);
                }
                else if range1.0 <= range2.1 && range1.1 >= range2.1 {
                    ranges[i] = (range2.0, range1.1);
                    ranges[j] = (0, 0);
                }
            }
        }

        for range in ranges {
            if range.0 != 0 || range.1 != 0 {
                result += range.1 - range.0 + 1;
            }
        }
    }


    println!("Result: {}", result);
    result
}


fn main() {
    for mode in [Part::Part1, Part::Part2] {
        for file in INPUT_FILES {
            let before = std::time::Instant::now();
            let input = fs::read_to_string(file).expect("Failed to read input file");
            let result = parse_input(&input, &mode);
            assert_eq!(result, RESULTS[INPUT_FILES.iter().position(|&f| f == file).unwrap()][match mode {
               Part::Part1 => 0,
               Part::Part2 => 1,
            }]);
            let after = std::time::Instant::now();
            let duration = after.duration_since(before);
            println!("Time taken for file {}: {:?}", file, duration);
        }
    }
}