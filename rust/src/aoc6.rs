use crate::helpers::Part;

pub const RESULTS: [[i64; 2]; 2] = [
    [4277556, 3263827],    // Expected results for example input: Part1, Part2
    [4951502530386, 8486156119946], // Expected results for actual data input: Part1, Part2
];

pub fn parse_input(input: &str, mode: &Part) -> i64 {
    let mut result: i64 = 0;

    let lines: Vec<&str> = input.lines().filter(|l| !l.is_empty()).collect();
    
    let operations: Vec<(usize, char)> = input.chars()
        .enumerate()
        .filter(|(_, c)| *c == '+' || *c == '*')
        .map(|(i, c)| (i, c))
        .collect();

    let mut col_offset = operations[0].0;
    
    for (op_idx, (_, op_char)) in operations.iter().enumerate() {
        let next_offset = if op_idx + 1 < operations.len() {
            operations[op_idx + 1].0
        } else {
            input.len() + 1
        };
        
        let mut op_result: i64 = if *op_char == '+' { 0 } else { 1 };
        let width = next_offset - col_offset - 1;
        let current_col_offset = col_offset - operations[0].0;
        
        match mode {
            Part::Part1 => {
                for line in &lines {
                    // Only select the number in the current column (col_offset)
                    let num_str = line.chars().skip(current_col_offset).take(width).collect::<String>();
                    if let Ok(num) = num_str.trim().parse::<i64>() {
                        if *op_char == '+' {
                            op_result += num;
                        } else {
                            op_result *= num;
                        }
                    }
                }
            }
            Part::Part2 => {
                for j in 0..width {
                    let mut num: i64 = 0;
                    for line in &lines {
                        if j + current_col_offset < line.len() {
                            if let Some(d) = line.chars().nth(j + current_col_offset).and_then(|c| c.to_digit(10)) {
                                num = num * 10 + d as i64;
                            }
                        }
                    }
                    if *op_char == '+' {
                        op_result += num;
                    } else {
                        op_result *= num;
                    }
                }
            }
        }
        
        result += op_result;
        col_offset = next_offset;
    }
    result
}
