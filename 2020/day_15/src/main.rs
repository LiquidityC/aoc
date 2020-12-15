use std::collections::BTreeMap;

struct Memorizer {
    index: u32,
    last: Option<u32>,
    history: BTreeMap<u32, u32>,
}

impl Memorizer {
    pub fn new() -> Self {
        Self {
            index: 1,
            last: None,
            history: BTreeMap::new(),
        }
    }

    pub fn get_next(&mut self) -> u32 {
        if let Some(last) = &self.last {
            if let Some(last_index) = self.history.get(last) {
                self.index - last_index
            } else {
                0
            }
        } else {
            panic!("You need to push a number before you can generate. Idiot!");
        }
    }

    pub fn push(&mut self, next: &u32) {
        if let Some(last) = &self.last {
            self.history.insert(*last, self.index);
            self.index += 1;
        }
        self.last = Some(*next);
    }
}

fn main() {
    let start_numbers: Vec<u32> = input_reader::get_inputs_split(",");
    let mut mem = Memorizer::new();

    start_numbers.iter().for_each(|n| mem.push(n));

    while mem.index < 2020 {
        let next = mem.get_next();
        mem.push(&next);
    }

    println!("Part 1: {}", mem.last.unwrap());
    while mem.index < 30000000 {
        let next = mem.get_next();
        mem.push(&next);
    }

    println!("Part 2: {}", mem.last.unwrap());
}
