use crate::helpers::Part;

pub const RESULTS: [[i64; 2]; 2] = [
    [40, 25272],    // Expected results for example input: Part1, Part2
    [69192, 7264308110], // Expected results for actual data input: Part1, Part2
];

#[derive(PartialEq)]
struct Point {
    x: i64,
    y: i64,
    z: i64,
    in_circuit: bool,
}

struct Line {
    start: usize,
    end: usize,
    distance: i64,
}

#[derive(Clone)]
struct Circuit {
    points: Vec<usize>
}

pub fn parse_input(input: &str, mode: &Part) -> i64 {
    let lines: Vec<&str> = input.lines().filter(|l| !l.is_empty()).collect();
    let mut points: Vec<Point> = lines
        .iter()
        .map(|line| {
            let mut coords = line.split(',').map(|s| s.trim().parse::<i64>().unwrap());
            Point {
                x: coords.next().unwrap(),
                y: coords.next().unwrap(),
                z: coords.next().unwrap(),
                in_circuit: false,
            }
        })
        .collect();

    let mut line_segments: Vec<Line> = vec![];
    for i in 0..points.len() {
        for j in i + 1..points.len() {
            let p1 = &points[i];
            let p2 = &points[j];
            let distance = ((p1.x - p2.x).pow(2) + (p1.y - p2.y).pow(2) + (p1.z - p2.z).pow(2)) as i64;
            line_segments.push(Line { start: i, end: j, distance });
        }
    }

    line_segments.sort_by_key(|line| line.distance);

    let mut circuits: Vec<Circuit> = Vec::new();
    // Arbitrary threshold to differentiate between example and actual input
    let mut counter = if lines.len() > 20 { 1000 } else { 10 };
    let mut line_count = lines.len();

    for line in &line_segments {
        let (start_idx, end_idx) = (line.start, line.end);
        let (p1_in, p2_in) = (points[start_idx].in_circuit, points[end_idx].in_circuit);

        counter -= 1;

        match (p1_in, p2_in) {
            (false, false) => {
                circuits.push(Circuit { points: vec![start_idx, end_idx] });
                line_count -= 2;
                points[start_idx].in_circuit = true;
                points[end_idx].in_circuit = true;
            }
            (true, false) => {
                if let Some(circuit) = circuits.iter_mut().find(|c| c.points.contains(&start_idx)) {
                    circuit.points.push(end_idx);
                    line_count -= 1;
                    points[end_idx].in_circuit = true;
                } else {
                    assert!(false, "Start point is in a circuit but not found in any circuit");
                }
            }
            (false, true) => {
                if let Some(circuit) = circuits.iter_mut().find(|c| c.points.contains(&end_idx)) {
                    circuit.points.push(start_idx);
                    line_count -= 1;
                    points[start_idx].in_circuit = true;
                } else {
                    assert!(false, "End point is in a circuit but not found in any circuit");
                }
            }
            (true, true) => {
                let (c1, c2) = {
                    let idxs = circuits
                        .iter()
                        .enumerate()
                        .filter_map(|(i, c)| {
                            if c.points.contains(&start_idx) {
                                Some((i, "c1"))
                            } else if c.points.contains(&end_idx) {
                                Some((i, "c2"))
                            } else {
                                None
                            }
                        })
                        .collect::<Vec<_>>();
                    let c1 = idxs.iter().find(|(_, tag)| *tag == "c1").map(|(i, _)| *i);
                    let c2 = idxs.iter().find(|(_, tag)| *tag == "c2").map(|(i, _)| *i);
                    (c1, c2)
                };
                if let (Some(c1), Some(c2)) = (c1, c2) {
                    if c1 != c2 {
                        let c_t = circuits[c2].clone();
                        circuits[c1].points.extend(c_t.points.iter());
                        circuits.remove(c2);
                    }
                }
            }
        }

        if matches!(mode, Part::Part1) && counter == 0 {
            circuits.sort_by_key(|c| c.points.len());
            circuits.reverse();
            return circuits.iter().take(3).map(|c| c.points.len() as i64).product();
        } else if matches!(mode, Part::Part2) && line_count == 0 && circuits.len() == 1 {
            return points[start_idx].x * points[end_idx].x;
        }
    }

    0
}
