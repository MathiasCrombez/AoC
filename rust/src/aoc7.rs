use crate::helpers::Part;

pub const RESULTS: [[i64; 2]; 2] = [
    [21, 40],    // Expected results for example input: Part1, Part2
    [1628, 27055852018812], // Expected results for actual data input: Part1, Part2
];

pub fn parse_input(input: &str, mode: &Part) -> i64 {
    let mut result: i64 = 0;

    let lines: Vec<&str> = input.lines().filter(|l| !l.is_empty()).collect();

    let start = lines.first()
        .and_then(|line| line.chars().position(|c| c == 'S'))
        .unwrap_or(0);

    let mut lasers: Vec<usize> = vec![];
    lasers.push(start);
    let mut all_prisms: Vec<Vec<usize>> = vec![];


    for line in lines.iter().skip(2).step_by(2) {
        // find all prisms usable by current lasers
        all_prisms.push(line
            .char_indices()
            .filter_map(|(i, c)| if c == '^' { Some(i) } else { None })
            .filter(|i| lasers.contains(i))
            .collect()
        );
        // remove used lasers and add new ones from prisms
        lasers.retain(|l| !all_prisms.last().unwrap().contains(l));
        for prism in all_prisms.last().unwrap() {
            lasers.push(prism.saturating_sub(1));
            lasers.push(prism + 1);
            if matches!(mode, Part::Part1) {
                result += 1;
            }
        }
        // remove duplicates
        lasers.sort_unstable();
        lasers.dedup();
    }

    if matches!(mode, Part::Part2) {
        let mut combinations: Vec<usize> = vec![1; lines.first().unwrap().len()];
        combinations.fill(0);
        combinations[start] = 1;

        // each prism splits the current combinations into left and right paths
        for p in all_prisms {
            for i in p {
                combinations[i - 1] += combinations[i];
                combinations[i + 1] += combinations[i];
                combinations[i] = 0;
            }
        }

        for c in combinations {
            result += c as i64;
        }
    }

    result
}
