use std::collections::{HashMap, HashSet};

type Direction = (i32, i32);

fn lines_to_directions(lines: &[String]) -> Vec<Vec<Direction>> {
    let mut directions = vec![];
    for l in lines {
        let chars: Vec<char> = l.chars().collect();
        let mut dirs = vec![];
        let mut i = 0;
        while i < chars.len() {
            let c = chars[i];
            let mut x: i32 = 0;
            let mut y: i32 = 0;
            if c == 'n' {
                y = -1;
                x = if chars[i + 1] == 'e' { 0 } else { -1 };
                i += 1;
            } else if c == 's' {
                y = 1;
                x = if chars[i + 1] == 'e' { 1 } else { 0 };
                i += 1;
            } else if c == 'e' {
                x = 1;
            } else if c == 'w' {
                x = -1;
            }
            i += 1;
            dirs.push((x, y));
        }
        directions.push(dirs);
    }
    directions
}

fn part1(directions: &[Vec<Direction>]) -> HashSet<Direction> {
    let mut tiles = HashSet::<Direction>::new();
    for dirs in directions {
        let mut pos = (0, 0);
        for dir in dirs {
            pos = (pos.0 + dir.0, pos.1 + dir.1);
        }
        if tiles.contains(&pos) {
            tiles.remove(&pos);
        } else {
            tiles.insert(pos.clone());
        }
    }

    tiles
}

const DIRS: &[(i32, i32)] = &[(1, 0), (-1, 0), (0, 1), (1, 1), (-1, -1), (0, -1)];

fn part2(tiles: &mut HashSet<Direction>) {
    for _ in 0..100 {
        let mut neighbor_count: HashMap<Direction, usize> = HashMap::new();
        for t in tiles.iter() {
            let neighbors: Vec<(i32, i32)> = DIRS.iter().map(|d| (t.0 + d.0, t.1 + d.1)).collect();
            for n in neighbors {
                if let Some(count) = neighbor_count.get_mut(&n) {
                    *count += 1;
                } else {
                    neighbor_count.insert(n, 1);
                }
            }
        }
        let mut new_set = HashSet::new();
        for (n, c) in neighbor_count {
            if tiles.contains(&n) {
                if c == 1 || c == 2 {
                    new_set.insert(n);
                }
            } else if c == 2 {
                new_set.insert(n);
            }
        }
        *tiles = new_set;
    }
}

fn main() {
    let lines: Vec<String> = input_reader::get_inputs();
    let directions = lines_to_directions(&lines);
    let mut tiles = part1(&directions);
    println!("Part 1: {}", tiles.len());
    part2(&mut tiles);
    println!("Part 2: {}", tiles.len());
}
