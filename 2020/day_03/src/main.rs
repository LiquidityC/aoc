struct Slope {
    rows: Vec<String>,
}

impl Slope {
    fn new(rows: &[String]) -> Self {
        Self {
            rows: rows.to_owned(),
        }
    }

    fn get_cell(&self, x: usize, y: usize) -> Option<char> {
        if let Some(row) = self.rows.get(y) {
            let dx = x % row.len();
            row.chars().nth(dx)
        } else {
            None
        }
    }

    fn traverse(&self, dx: usize, dy: usize) -> u32 {
        let mut x = 0;
        let mut y = 0;

        let mut tree_count = 0;
        while let Some(c) = self.get_cell(x, y) {
            x += dx;
            y += dy;
            if c == '#' {
                tree_count += 1;
            }
        }
        tree_count
    }
}

fn part1(slope: &Slope) {
    println!("Part 1: {}", slope.traverse(3, 1));
}

fn part2(slope: &Slope) {
    let mut mul = 1;
    mul *= slope.traverse(1, 1);
    mul *= slope.traverse(3, 1);
    mul *= slope.traverse(5, 1);
    mul *= slope.traverse(7, 1);
    mul *= slope.traverse(1, 2);
    println!("Part 2: {}", mul);
}

fn main() {
    let input: Vec<String> = input_reader::get_inputs();
    let slope = Slope::new(&input);
    part1(&slope);
    part2(&slope);
}
