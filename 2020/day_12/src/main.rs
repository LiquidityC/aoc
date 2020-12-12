use std::str::FromStr;

#[derive(Debug)]
enum Move {
    Forward(u32),
    Turn(f32),
    East(u32),
    West(u32),
    North(u32),
    South(u32),
}

impl FromStr for Move {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let c = s.chars().next().unwrap();
        let val: u32 = s[1..].parse().expect("Unable to parse val");

        Ok(match c {
            'F' => Self::Forward(val),
            'L' => Self::Turn(val as f32),
            'R' => Self::Turn(-1.0 * val as f32),
            'E' => Self::East(val),
            'W' => Self::West(val),
            'N' => Self::North(val),
            'S' => Self::South(val),
            _ => unreachable!(),
        })
    }
}

#[derive(Debug)]
struct Ship {
    pos: (i32, i32),
    angle: f32,
    wp: (i32, i32),
}

impl Ship {
    fn new() -> Self {
        Self {
            pos: (0, 0),
            angle: 0.0,
            wp: (10, 1),
        }
    }

    fn turn(&mut self, angle: &f32) {
        self.angle += angle;
    }

    fn dir(&self) -> (i32, i32) {
        let x = self.angle.to_radians().cos();
        let y = self.angle.to_radians().sin();
        (x as i32, y as i32)
    }

    fn step(&mut self, val: &u32, dir: (i32, i32)) {
        self.pos.0 += dir.0 * *val as i32;
        self.pos.1 += dir.1 * *val as i32;
    }

    fn go_wp(&mut self, val: &u32) {
        self.pos.0 += *val as i32 * self.wp.0;
        self.pos.1 += *val as i32 * self.wp.1;
    }

    fn rotate_wp(&mut self, angle: &f32) {
        let s = angle.to_radians().sin() as i32;
        let c = angle.to_radians().cos() as i32;
        let xnew = self.wp.0 * c - self.wp.1 * s;
        let ynew = self.wp.0 * s + self.wp.1 * c;
        self.wp.0 = xnew;
        self.wp.1 = ynew;
    }

    fn step_wp(&mut self, val: &u32, dir: (i32, i32)) {
        self.wp.0 += dir.0 * *val as i32;
        self.wp.1 += dir.1 * *val as i32;
    }

    fn navigate_wp(&mut self, m: &Move) {
        match m {
            Move::Forward(v) => self.go_wp(v),
            Move::Turn(v) => self.rotate_wp(v),
            Move::East(v) => self.step_wp(v, (1, 0)),
            Move::West(v) => self.step_wp(v, (-1, 0)),
            Move::North(v) => self.step_wp(v, (0, 1)),
            Move::South(v) => self.step_wp(v, (0, -1)),
        }
    }

    fn navigate(&mut self, m: &Move) {
        match m {
            Move::Forward(v) => self.step(v, self.dir()),
            Move::Turn(v) => self.turn(v),
            Move::East(v) => self.step(v, (1, 0)),
            Move::West(v) => self.step(v, (-1, 0)),
            Move::North(v) => self.step(v, (0, 1)),
            Move::South(v) => self.step(v, (0, -1)),
        }
    }
}

fn main() {
    let moves: Vec<Move> = input_reader::get_inputs();
    let mut ship = Ship::new();
    for m in &moves {
        ship.navigate(&m);
    }
    println!("Part 1: {}", ship.pos.0.abs() + ship.pos.1.abs());
    ship = Ship::new();
    for m in &moves {
        ship.navigate_wp(&m);
    }
    println!("Part 2: {}", ship.pos.0.abs() + ship.pos.1.abs());
}
