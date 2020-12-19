use std::{collections::BTreeMap, error::Error};

use regex::Regex;

type RuleMap = BTreeMap<u32, Rule>;

#[derive(Debug, Clone)]
struct Rule {
    id: u32,
    pattern: Option<String>,
    sub: Option<Vec<u32>>,
    alt: Option<Vec<u32>>,
}

impl std::str::FromStr for Rule {
    type Err = Box<dyn Error>;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut split = s.split(": ");
        let id = split.next().unwrap().parse::<u32>()?;
        let body = split.next().unwrap();

        let re_char = Regex::new("^\"(.)\"$")?;
        let re_sub = Regex::new(r"^([\d ]+)$")?;
        let re_sub_alt = Regex::new(r"^([\d ]+?) \| ([\d+ ]+?)$")?;

        let mut pattern = None;
        let mut sub = None;
        let mut alt = None;

        if let Some(cap) = re_char.captures(&body) {
            pattern = Some(cap[1].to_string())
        } else if let Some(cap) = re_sub.captures(&body) {
            sub = Some(
                cap.get(1)
                    .unwrap()
                    .as_str()
                    .split(' ')
                    .map(|n| n.parse::<u32>().unwrap())
                    .collect(),
            );
        } else if let Some(cap) = re_sub_alt.captures(&body) {
            sub = Some(
                cap.get(1)
                    .unwrap()
                    .as_str()
                    .split(' ')
                    .map(|n| n.parse::<u32>().unwrap())
                    .collect(),
            );
            alt = Some(
                cap.get(2)
                    .unwrap()
                    .as_str()
                    .split(' ')
                    .map(|n| n.parse::<u32>().unwrap())
                    .collect(),
            );
        } else {
            panic!("Bad parsing");
        }

        Ok(Self {
            id,
            pattern,
            sub,
            alt,
        })
    }
}

impl Rule {
    fn to_pattern(&self, rules: &RuleMap) -> String {
        if let Some(pattern) = &self.pattern {
            pattern.clone()
        } else if self.alt.is_some() {
            let sub = self.sub.clone().unwrap();
            let alt = self.alt.clone().unwrap();
            format!(
                "({}|{})",
                sub.iter()
                    .map(|id| rules.get(id).unwrap())
                    .map(|r| r.to_pattern(rules))
                    .collect::<String>(),
                alt.iter()
                    .map(|id| rules.get(id).unwrap())
                    .map(|r| r.to_pattern(rules))
                    .collect::<String>(),
            )
        } else {
            let sub = self.sub.clone().unwrap();
            sub.iter()
                .map(|id| rules.get(id).unwrap())
                .map(|r| r.to_pattern(rules))
                .collect::<String>()
        }
    }
}

fn get_rules(input: &str) -> RuleMap {
    let mut rules = RuleMap::new();
    input.lines().for_each(|l| {
        let r: Rule = l.parse().unwrap();
        rules.insert(r.id, r);
    });
    rules
}

fn main() {
    let input: Vec<String> = input_reader::get_inputs_split("\n\n");
    let mut rules = get_rules(&input[0]);
    let lines = input[1].clone();
    let r0 = &rules.get(&0).clone().unwrap();
    let re = Regex::new(&format!("^{}$", &r0.to_pattern(&rules))).unwrap();
    println!(
        "Part 1: {}",
        lines.lines().filter(|l| re.is_match(l)).count()
    );

    rules.insert(8, "8: 42 | 42 8".parse::<Rule>().unwrap());
    rules.insert(11, "11: 42 31 | 42 11 31".parse::<Rule>().unwrap());

    let p42 = rules.get(&42).unwrap().to_pattern(&rules);
    let r42 = Regex::new(&format!("^{}", p42)).unwrap();
    let p31 = rules.get(&31).unwrap().to_pattern(&rules);
    let r31 = Regex::new(&format!("{}$", p31)).unwrap();

    let mut mc = 0;
    let filter = Regex::new(&format!("^{}+{}+$", p42, p31)).unwrap();
    lines.lines().for_each(|l| {
        if filter.is_match(&l) {
            let mut c42 = 0;
            let mut line = l.to_string();
            while r42.is_match(&line) {
                line = r42.replace(&line, "").to_string();
                c42 += 1;
            }
            let mut c31 = 0;
            while r31.is_match(&line) {
                line = r31.replace(&line, "").to_string();
                c31 += 1;
            }
            if c42 > 0 && c31 > 0 && c42 > c31 {
                mc += 1;
            }
        }
    });

    println!("Part 2: {}", mc);
}
