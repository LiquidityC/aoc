use std::{error::Error, str::FromStr};

use text_io::scan;

struct PasswordRule {
    character: char,
    password: String,
    min: u32,
    max: u32,
}

impl PasswordRule {
    fn is_valid(&self) -> bool {
        let count = self.password.chars().filter(|c| *c == self.character).count() as u32;
        return count >= self.min && count <= self.max;
    }

    fn alternate_valid(&self) -> bool {
        let first_pos = self.password.chars().nth(self.min as usize - 1).unwrap() == self.character;
        let second_pos =
            self.password.chars().nth(self.max as usize - 1).unwrap() == self.character;
        first_pos ^ second_pos
    }
}

impl FromStr for PasswordRule {
    type Err = Box<dyn Error>;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let character: char;
        let min: u32;
        let max: u32;
        let password: String;
        scan!(s.bytes() => "{}-{} {}: {}", min, max, character, password);

        Ok(Self {
            character,
            password,
            min,
            max,
        })
    }
}

fn part1(input: &[PasswordRule]) {
    println!(
        "Part 1: {}",
        input.iter().filter(|pwd| pwd.is_valid()).count()
    );
}

fn part2(input: &[PasswordRule]) {
    println!(
        "Part 2: {}",
        input.iter().filter(|pwd| pwd.alternate_valid()).count()
    );
}

fn main() {
    let input: Vec<PasswordRule> = input_reader::get_inputs();
    part1(&input);
    part2(&input);
}
