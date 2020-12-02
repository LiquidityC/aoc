use std::{error::Error, str::FromStr};

use regex::Regex;

struct PasswordRule {
    character: char,
    password: String,
    min: u32,
    max: u32,
}

impl PasswordRule {
    fn is_valid(&self) -> bool {
        let mut count = 0;
        for c in self.password.chars() {
            if c == self.character {
                count += 1;
            }
        }

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
        let regex = Regex::new("^(\\d+)-(\\d+) (.): (.+)$")?;
        let captures = regex.captures(s).unwrap();

        //println!("{:?}", &captures);
        let character = captures[3].chars().next().unwrap();
        let min: u32 = captures[1].parse()?;
        let max: u32 = captures[2].parse()?;
        let password = captures[4].to_string();

        Ok(Self {
            character,
            password,
            min,
            max,
        })
    }
}

fn part1(input: &Vec<PasswordRule>) {
    println!(
        "Part 1: {}",
        input.iter().filter(|pwd| pwd.is_valid()).count()
    );
}

fn part2(input: &Vec<PasswordRule>) {
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
