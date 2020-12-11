use std::collections::HashMap;

#[derive(Clone)]
struct Seats {
    m: Vec<Vec<char>>,
}

impl std::ops::Deref for Seats {
    type Target = Vec<Vec<char>>;

    fn deref(&self) -> &Self::Target {
        &self.m
    }
}

impl std::ops::DerefMut for Seats {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.m
    }
}

impl std::str::FromStr for Seats {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut rows = vec![];
        for l in s.lines() {
            rows.push(l.chars().collect());
        }
        Ok(Self { m: rows })
    }
}

impl Seats {
    fn simulate1(&mut self) {
        loop {
            if !self.cycle1() {
                break;
            }
        }
    }

    fn simulate2(&mut self) {
        loop {
            if !self.cycle2() {
                break;
            }
        }
    }

    fn occupied_count(&self) -> usize {
        self.m.iter().flatten().filter(|c| **c == '#').count()
    }

    fn cycle1(&mut self) -> bool {
        let mut changes: HashMap<(usize, usize), char> = HashMap::new();
        let directions: Vec<(i32, i32)> = vec![
            (1, 0),
            (-1, 0),
            (0, 1),
            (0, -1),
            (1, 1),
            (1, -1),
            (-1, 1),
            (-1, -1),
        ];
        for (y, row) in self.m.iter().enumerate() {
            for (x, c) in row.iter().enumerate() {
                let x = x as i32;
                let y = y as i32;
                if *c == 'L' {
                    let count = directions
                        .iter()
                        .map(|(dy, dx)| self.get(y + dy, x + dx))
                        .filter(|s| *s != '#')
                        .count();
                    if count == 8 {
                        changes.insert((y as usize, x as usize), '#');
                    }
                } else if *c == '#' {
                    let count = directions
                        .iter()
                        .map(|(dy, dx)| self.get(y + dy, x + dx))
                        .filter(|s| *s == '#')
                        .count();
                    if count >= 4 {
                        changes.insert((y as usize, x as usize), 'L');
                    }
                }
            }
        }

        for (pos, c) in changes.iter() {
            self.m[pos.0][pos.1] = *c;
        }

        !changes.is_empty()
    }

    fn cycle2(&mut self) -> bool {
        let mut changes: HashMap<(usize, usize), char> = HashMap::new();
        let directions: Vec<(i32, i32)> = vec![
            (1, 0),
            (-1, 0),
            (0, 1),
            (0, -1),
            (1, 1),
            (1, -1),
            (-1, 1),
            (-1, -1),
        ];
        for (y, row) in self.m.iter().enumerate() {
            for (x, c) in row.iter().enumerate() {
                let x = x as i32;
                let y = y as i32;
                if *c == 'L' {
                    let count = directions
                        .iter()
                        .map(|dir| self.scan((y, x), dir))
                        .filter(|s| *s != '#')
                        .count();
                    if count == 8 {
                        changes.insert((y as usize, x as usize), '#');
                    }
                } else if *c == '#' {
                    let count = directions
                        .iter()
                        .map(|dir| self.scan((y, x), dir))
                        .filter(|s| *s == '#')
                        .count();
                    if count >= 5 {
                        changes.insert((y as usize, x as usize), 'L');
                    }
                }
            }
        }

        for (pos, c) in changes.iter() {
            self.m[pos.0][pos.1] = *c;
        }

        !changes.is_empty()
    }

    fn scan(&self, origin: (i32, i32), dir: &(i32, i32)) -> char {
        let (mut y, mut x) = origin;
        let (dy, dx) = dir;

        let mut result: char;
        loop {
            y += dy;
            x += dx;

            result = self.get(y, x);
            if result != '.' {
                break;
            }
        }
        if result == '_' {
            result = 'L'
        }
        result
    }

    fn get(&self, y: i32, x: i32) -> char {
        let h = self.m.len() as i32;
        let w = self.m[0].len() as i32;
        if (x < 0 || x >= w) || (y < 0 || y >= h) {
            '_'
        } else {
            self.m[y as usize][x as usize]
        }
    }
}

fn main() {
    let mut seats: Seats = input_reader::get_input();
    seats.simulate1();
    println!("Part 1: {}", seats.occupied_count());

    let mut seats: Seats = input_reader::get_input();
    seats.simulate2();
    println!("Part 2: {}", seats.occupied_count());
}
