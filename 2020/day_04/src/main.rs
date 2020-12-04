use std::{collections::HashMap, str::FromStr};

use regex::Regex;

struct Passport {
    params: HashMap<String, String>,
}

impl FromStr for Passport {
    type Err = Box<dyn std::error::Error>;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let entries: Vec<String> = s
            .replace("\n", " ")
            .split(' ')
            .map(|s| s.to_string())
            .collect();
        let mut params = HashMap::new();
        for e in entries {
            let mut split = e.split(':');
            params.insert(
                split.next().unwrap().to_string(),
                split.next().unwrap().to_string(),
            );
        }
        Ok(Self { params })
    }
}

impl Passport {
    fn is_valid(&self) -> bool {
        let keys = vec!["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"];
        let mut result = true;
        for key in keys {
            if !self.params.contains_key(key) {
                result = false
            }
        }
        result
    }

    fn is_really_valid(&self) -> bool {
        let mut valid = self.is_valid();
        if valid {
            for (key, val) in &self.params {
                match key.as_str() {
                    "byr" => {
                        let year: u32 = val.parse().unwrap_or(0);
                        if year < 1920 || year > 2002 {
                            valid = false;
                            break;
                        }
                    }
                    "iyr" => {
                        let year: u32 = val.parse().unwrap_or(0);
                        if year < 2010 || year > 2020 {
                            valid = false;
                            break;
                        }
                    }
                    "eyr" => {
                        let year: u32 = val.parse().unwrap_or(0);
                        if year < 2020 || year > 2030 {
                            valid = false;
                            break;
                        }
                    }
                    "hgt" => {
                        let height = val[0..val.len() - 2].to_string();
                        let unit = val[val.len() - 2..].to_string();
                        let height: u32 = height.parse().unwrap_or(0);
                        if unit == "cm" {
                            if height < 150 || height > 193 {
                                valid = false;
                                break;
                            }
                        } else if unit == "in" {
                            if height < 59 || height > 76 {
                                valid = false;
                                break;
                            }
                        } else {
                            valid = false;
                            break;
                        }
                    }
                    "hcl" => {
                        let re = Regex::new(r"^#[0-9a-fA-F]{6}+$").unwrap();
                        if !re.is_match(&val) {
                            valid = false;
                            break;
                        }
                    }
                    "ecl" => {
                        if !matches!(
                            val.as_str(),
                            "amb" | "blu" | "brn" | "gry" | "grn" | "hzl" | "oth"
                        ) {
                            valid = false;
                            break;
                        }
                    }
                    "pid" => {
                        let re = Regex::new(r"^[0-9]{9}$").unwrap();
                        if !re.is_match(&val) {
                            valid = false;
                            break;
                        }
                    }
                    _ => {}
                }
            }
        }
        valid
    }
}

fn part1(passports: &[Passport]) {
    println!(
        "Part1: {}",
        passports.iter().filter(|p| p.is_valid()).count()
    );
}

fn part2(passports: &[Passport]) {
    println!(
        "Part2: {}",
        passports.iter().filter(|p| p.is_really_valid()).count()
    );
}

fn main() {
    let input: Vec<Passport> = input_reader::get_inputs_split("\n\n");

    part1(&input);
    part2(&input);
}
