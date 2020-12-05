use std::collections::HashMap;

struct NavMap {
    position: (i32, i32),
    visited: HashMap<(i32, i32), u32>,
}

impl NavMap {
    fn new() -> Self {
        Self {
            position: (0, 0),
            visited: HashMap::new(),
        }
    }

    fn walk(&mut self, path: &[(i32, i32)]) {
        self.visited.insert(self.position, 1);
        for dir in path {
            self.position = (self.position.0 + dir.0, self.position.1 + dir.1);
            if let Some(val) = self.visited.get_mut(&self.position) {
                *val += 1;
            } else {
                self.visited.insert(self.position, 1);
            }
        }
    }

    fn visited(&self) -> HashMap<(i32, i32), u32> {
        self.visited.clone()
    }
}

fn main() {
    let input: String = input_reader::get_input();
    let path: Vec<(i32, i32)> = input
        .chars()
        .map(|c| match c {
            '^' => (0, 1),
            'v' => (0, -1),
            '<' => (-1, 0),
            '>' => (1, 0),
            _ => panic!("Bad input data"),
        })
        .collect();
    let mut santa = NavMap::new();
    santa.walk(&path);
    println!("Part 1: {}", santa.visited().len());

    let mut santa_path = vec![];
    let mut robo_path = vec![];
    for dir in path.iter().step_by(2) {
        santa_path.push(*dir);
    }
    for dir in path[1..].iter().step_by(2) {
        robo_path.push(*dir);
    }

    let mut santa = NavMap::new();
    santa.walk(&santa_path);
    let mut robo = NavMap::new();
    robo.walk(&robo_path);

    let mut visited = santa.visited();
    visited.extend(robo.visited());
    println!("Part 2: {}", visited.len());
}
