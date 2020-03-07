use std::fs;

#[derive(Debug)]
struct Coordinate {
    pub x: i32,
    pub y: i32,
}

impl Coordinate {
    fn new(x: i32, y: i32) -> Coordinate {
        Coordinate { x, y }
    }

    fn manhattan_distance(&self, x: i32, y: i32) -> u32 {
        let result = (self.x - x).abs() + (self.y - y).abs();
        result as u32
    }

    fn equals(&self, other: &Coordinate) -> bool {
        self.x == other.x && self.y == other.y
    }
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Unable to read input file");

    let input = String::from(input.trim());
    let coordinates: Vec<Coordinate> = input
        .split("\n")
        .map(|entry: &str| -> Coordinate {
            let pair: Vec<&str> = entry.split(", ").collect();
            let (sx, sy) = (pair[0], pair[1]);
            Coordinate::new(sx.parse().unwrap(), sy.parse().unwrap())
        })
        .collect();

    let (mut max_x, mut max_y) = (0, 0);
    for c in &coordinates {
        max_x = max_x.max(c.x);
        max_y = max_y.max(c.y);
    }

    let mut best_score = 0;
    for c in &coordinates {
        best_score = best_score.max(check_score(&c, &coordinates, max_x, max_y).unwrap_or(0));
    }
    println!("Part 1: {}", best_score);
}

fn check_score(coord: &Coordinate, coords: &Vec<Coordinate>, max_x: i32, max_y: i32) -> Result<u32, &'static str> {
    let mut score = 0;
    for x in 0..max_x {
        for y in 0..max_y {
            let distance = coord.manhattan_distance(x, y);
            if coords
                .iter()
                .filter(|&other| !other.equals(coord))
                .all(|other| other.manhattan_distance(x, y) > distance)
            {
                score = score + 1;
                if x == 0 || y  == 0 || x == max_x - 1 || y == max_y - 1 {
                    return Err("")
                }
            }
        }
    }
    Ok(score)
}
