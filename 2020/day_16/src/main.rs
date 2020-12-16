use std::ops::RangeInclusive;
use text_io::scan;

#[derive(Clone)]
struct Ticket {
    inner: Vec<u64>,
}

impl std::ops::Deref for Ticket {
    type Target = Vec<u64>;

    fn deref(&self) -> &Self::Target {
        &self.inner
    }
}

impl std::str::FromStr for Ticket {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if !s.contains(',') {
            Err("Invalid line".to_string())
        } else {
            Ok(Self {
                inner: s.split(',').map(|n| n.parse::<u64>().unwrap()).collect(),
            })
        }
    }
}

impl Ticket {
    fn invalid_num(&self, rules: &[Rule]) -> Option<u64> {
        let mut result = None;
        for n in &self.inner {
            if rules.iter().all(|r| !r.valid(&n)) {
                result = Some(*n);
                break;
            }
        }
        result
    }

    fn validate_rule(&self, rule: &Rule) -> Vec<bool> {
        self.iter().map(|n| rule.valid(n)).collect()
    }
}

#[derive(Debug, Clone)]
struct Rule {
    name: String,
    rules: (RangeInclusive<u64>, RangeInclusive<u64>),
    validations: Vec<bool>,
}

impl std::str::FromStr for Rule {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let name: String;
        let r1_s: u64;
        let r1_e: u64;
        let r2_s: u64;
        let r2_e: u64;

        scan!(s.bytes() => "{}: {}-{} or {}-{}", name, r1_s, r1_e, r2_s, r2_e);
        let r1 = RangeInclusive::new(r1_s, r1_e);
        let r2 = RangeInclusive::new(r2_s, r2_e);

        Ok(Self {
            name,
            rules: (r1, r2),
            validations: vec![true; 20],
        })
    }
}

impl Rule {
    fn valid(&self, num: &u64) -> bool {
        self.rules.0.contains(num) || self.rules.1.contains(num)
    }

    fn merge_validation(&mut self, validation: &[bool]) {
        self.validations = self
            .validations
            .iter()
            .zip(validation.iter())
            .map(|(a, b)| *a && *b)
            .collect();
    }

    fn merge_validation_excluding(&mut self, validation: &[bool]) {
        self.validations = self
            .validations
            .iter()
            .zip(validation.iter())
            .map(|(a, b)| if *a && *b { false } else { *a })
            .collect();
    }

    fn get_position(&self) -> usize {
        assert_eq!(self.validations.iter().filter(|v| **v).count(), 1);
        self.validations
            .iter()
            .enumerate()
            .find(|(_, v)| **v)
            .unwrap()
            .0
    }
}

fn main() {
    let data: Vec<String> = input_reader::get_inputs_split("\n\n");

    let mut rules: Vec<Rule> = data[0]
        .lines()
        .map(|l| l.parse::<Rule>().unwrap())
        .collect();

    let ticket: Ticket = data[1].lines().nth(1).unwrap().parse::<Ticket>().unwrap();

    let tickets: Vec<Ticket> = data[2]
        .lines()
        .map(|l| l.parse::<Ticket>())
        .filter(|r| r.is_ok())
        .map(|r| r.unwrap())
        .collect();
    println!(
        "Part 1: {}",
        tickets
            .iter()
            .map(|t| t.invalid_num(&rules))
            .filter(|r| r.is_some())
            .map(|r| r.unwrap())
            .sum::<u64>()
    );

    let mut valid_tickets: Vec<Ticket> = tickets
        .iter()
        .filter(|t| t.invalid_num(&rules).is_none())
        .cloned()
        .collect();
    valid_tickets.push(ticket.clone());
    assert_eq!(
        0,
        valid_tickets
            .iter()
            .map(|t| t.invalid_num(&rules))
            .filter(|r| r.is_some())
            .map(|r| r.unwrap())
            .sum::<u64>()
    );

    for r in rules.iter_mut() {
        for t in valid_tickets.iter() {
            r.merge_validation(&t.validate_rule(&r));
        }
    }
    rules.sort_by(|a, b| {
        a.validations
            .iter()
            .filter(|v| **v)
            .count()
            .cmp(&b.validations.iter().filter(|v| **v).count())
    });

    let res = rules
        .into_iter()
        .fold(Vec::<Rule>::new(), |mut acc, mut r| {
            if acc.is_empty() {
                acc.push(r);
                acc
            } else {
                for prev in acc.iter() {
                    r.merge_validation_excluding(&prev.validations);
                }
                acc.push(r);
                acc
            }
        });

    let positions: Vec<usize> = res
        .iter()
        .filter(|r| r.name.starts_with("departure"))
        .map(|r| r.get_position())
        .collect();
    println!(
        "Part 2: {}",
        positions.iter().map(|p| ticket[*p]).product::<u64>()
    );
}
