use std::collections::{BTreeMap, BTreeSet};

static DIRS: &[(i32, i32, i32)] = &[
    (-1, -1, -1),
    (0, -1, -1),
    (1, -1, -1),
    (-1, 0, -1),
    (0, 0, -1),
    (1, 0, -1),
    (-1, 1, -1),
    (0, 1, -1),
    (1, 1, -1),
    (-1, -1, 0),
    (0, -1, 0),
    (1, -1, 0),
    (-1, 0, 0),
    (1, 0, 0),
    (-1, 1, 0),
    (0, 1, 0),
    (1, 1, 0),
    (-1, -1, 1),
    (0, -1, 1),
    (1, -1, 1),
    (-1, 0, 1),
    (0, 0, 1),
    (1, 0, 1),
    (-1, 1, 1),
    (0, 1, 1),
    (1, 1, 1),
];

type Pos = (i32, i32, i32);
type Space = BTreeSet<Pos>;

struct Cube {
    active_cubes: Space,
}

impl std::str::FromStr for Cube {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut map: Space = Space::new();
        for (y, l) in s.lines().enumerate() {
            for (x, c) in l.char_indices() {
                if c == '#' {
                    let x = x as i32;
                    let y = y as i32;
                    map.insert((x, y, 0));
                }
            }
        }
        Ok(Self { active_cubes: map })
    }
}

impl Cube {
    fn cycle(&mut self) {
        let mut neighbors: BTreeMap<Pos, u32> = BTreeMap::new();
        for c in &self.active_cubes {
            for d in DIRS {
                let neighbor = (c.0 + d.0, c.1 + d.1, c.2 + d.2);
                if let Some(n) = neighbors.get_mut(&neighbor) {
                    *n += 1;
                } else {
                    neighbors.insert(neighbor, 1);
                }
            }
        }

        let mut next_state: Space = Space::new();
        for (n, count) in neighbors.into_iter() {
            match count {
                3 => {
                    next_state.insert(n);
                }
                2 => {
                    if self.active_cubes.contains(&n) {
                        next_state.insert(n);
                    }
                }
                _ => {}
            }
        }
        self.active_cubes = next_state;
    }
}

type Pos4d = (i32, i32, i32, i32);
type Space4d = BTreeSet<Pos4d>;

struct Dimension {
    active_spaces: Space4d,
}

impl std::str::FromStr for Dimension {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut map = Space4d::new();
        for (y, l) in s.lines().enumerate() {
            for (x, c) in l.char_indices() {
                if c == '#' {
                    let x = x as i32;
                    let y = y as i32;
                    map.insert((x, y, 0, 0));
                }
            }
        }
        Ok(Self { active_spaces: map })
    }
}

fn directions_4d() -> Vec<Pos4d> {
    let mut dirs: Vec<Pos4d> = vec![];
    for i in &[-1, 0, 1] {
        for d in DIRS {
            dirs.push((d.0, d.1, d.2, *i));
        }
    }
    dirs.push((0, 0, 0, -1));
    dirs.push((0, 0, 0, 1));
    assert_eq!(dirs.len(), 80);
    dirs
}

impl Dimension {
    fn cycle(&mut self) {
        let mut neighbors: BTreeMap<Pos4d, u32> = BTreeMap::new();
        for c in &self.active_spaces {
            for d in directions_4d() {
                let neighbor = (c.0 + d.0, c.1 + d.1, c.2 + d.2, c.3 + d.3);
                if let Some(n) = neighbors.get_mut(&neighbor) {
                    *n += 1;
                } else {
                    neighbors.insert(neighbor, 1);
                }
            }
        }

        let mut next_state: Space4d = Space4d::new();
        for (n, count) in neighbors.into_iter() {
            match count {
                3 => {
                    next_state.insert(n);
                }
                2 => {
                    if self.active_spaces.contains(&n) {
                        next_state.insert(n);
                    }
                }
                _ => {}
            }
        }
        self.active_spaces = next_state;
    }
}

fn part1(mut cube: Cube) {
    for _ in 0..6 {
        cube.cycle();
    }
    println!("Part 1: {}", cube.active_cubes.len());
}

fn part2(mut dim: Dimension) {
    for _ in 0..6 {
        dim.cycle();
    }
    println!("Part 2: {}", dim.active_spaces.len());
}

fn main() {
    part1(input_reader::get_input());
    part2(input_reader::get_input());
}
