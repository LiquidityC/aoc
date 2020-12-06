use std::collections::HashMap;

struct Group {
    size: u32,
    answers: HashMap<char, u32>,
}

impl Group {
    fn get_common_answers(&self) -> u32 {
        self.answers.values().filter(|v| *v == &self.size).count() as u32
    }
}

impl std::str::FromStr for Group {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut answers = HashMap::new();
        let mut size: u32 = 0;
        for l in s.lines() {
            size += 1;
            for c in l.chars() {
                if let Some(v) = answers.get_mut(&c) {
                    *v += 1;
                } else {
                    answers.insert(c, 1);
                }
            }
        }
        Ok(Self { size, answers })
    }
}

fn main() {
    let groups: Vec<Group> = input_reader::get_inputs_split("\n\n");

    let positive_answers: u32 = groups.iter().map(|g| g.answers.len() as u32).sum();
    println!("Part 1: {}", positive_answers);
    let common_answers: u32 = groups.iter().map(|g| g.get_common_answers()).sum();
    println!("Part 2: {}", common_answers);
}
