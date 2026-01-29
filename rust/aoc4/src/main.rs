use std::fs;

const INPUT_FILES : [&str; 2] = [
    "../../aoc4_ex.txt",
    "../../aoc4_data.txt",
];

enum Part {
    Part1,
    Part2,
}

const RESULTS: [[i64; 2]; 2] = [
    [13, 43],    // Expected results for example input: Part1, Part2
    [1424, 8727], // Expected results for actual data input: Part1, Part2
];

fn parse_input(input: &str, mode: &Part) -> i64 {

    let mut result: i64 = 0;

    let lines_nb = input.lines().map(|line| line.trim()).filter(|line| !line.is_empty()).count();
    let columns_nb = input.lines().next().unwrap().trim().len();

    let mut grid: Vec<Vec<u8>> = vec![vec![0; columns_nb]; lines_nb];

    for (i, line) in input.lines().map(|line| line.trim()).filter(|line| !line.is_empty()).enumerate() {
        for (j, ch) in line.chars().enumerate() {
            grid[i][j] = if ch == '@' { 1 } else { 0 };
        }
    }

    loop {
        let mut changed = false;
        for i in 0..lines_nb {
            for j in 0..columns_nb {
                if grid[i][j] != 1 {
                    continue;
                }
                
                // saturating_sub does i - 1 but does not go below 0, useful for boundaries without risking negative indices
                // which is not allowed in Rust
                let line_start = i.saturating_sub(1);
                let line_end = (i + 1).min(lines_nb - 1);
                let column_start = j.saturating_sub(1);
                let column_end = (j + 1).min(columns_nb - 1);

                // Note: this method to iterate is a twice slower than for loops, but it's more concise
                // This is due to the creation of intermediate iterators and early closures
                
                // iterate over the surrounding cells
                // filter out the center cell (i, j)
                // count how many surrounding cells are occupied (1) for Part1, empty (0) for Part2
                let count = (line_start..=line_end)
                    .flat_map(|l| (column_start..=column_end).map(move |m| (l, m)))
                    .filter(|&(l, m)| (l, m) != (i, j))
                    .filter(|&(l, m)| match mode {
                        Part::Part1 => grid[l][m] != 0,
                        Part::Part2 => grid[l][m] == 1,
                    })
                    .count();
                
                if count < 4 {
                    grid[i][j] = 2;
                    changed = true;
                }
            }
        }
        
        // For Part1, we stop after the first iteration
        // For Part2, we continue until no changes occur
        if matches!(mode, Part::Part1) || !changed {
            break;
        }
    }

    for i in 0..lines_nb {
        for j in 0..columns_nb {
            if grid[i][j] == 2 {
                result += 1;
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