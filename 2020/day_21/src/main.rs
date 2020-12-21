use std::collections::{HashMap, HashSet};

use regex::Regex;

type AllergenMap = HashMap<String, HashSet<String>>;
type FreqMap = HashMap<String, usize>;

fn part1(allerg_to_ings: &AllergenMap, ings_freq: &FreqMap) -> usize {
    let mut not_allergen_ings = ings_freq.keys().cloned().collect::<HashSet<String>>();
    allerg_to_ings.values().for_each(|ings| {
        not_allergen_ings = not_allergen_ings.difference(ings).cloned().collect();
    });
    not_allergen_ings
        .iter()
        .map(|ing| ings_freq.get(ing).unwrap())
        .sum()
}

fn part2(mut allerg_to_ings: AllergenMap) -> String {
    while allerg_to_ings.values().any(|ings| ings.len() > 1) {
        let allergen = allerg_to_ings
            .values()
            .find(|ings| ings.len() == 1)
            .unwrap()
            .clone();
        allerg_to_ings = allerg_to_ings
            .into_iter()
            .map(|(i, s)| match s.len() {
                1 => (i, s),
                _ => (i, s.difference(&allergen).cloned().collect()),
            })
            .collect();
    }

    let mut matches: Vec<(String, String)> = allerg_to_ings
        .iter()
        .map(|(k, v)| (k.clone(), v.iter().next().unwrap().clone()))
        .collect();
    matches.sort();
    matches.iter().fold(String::new(), |can, i| {
        if can.is_empty() {
            i.1.clone()
        } else {
            format!("{},{}", can, i.1)
        }
    })
}

fn main() {
    let re = Regex::new(r"^(.*?) \(contains (.*)\)$").unwrap();

    let mut ing_freq = FreqMap::new();
    let mut allerg_to_ing = AllergenMap::new();

    input_reader::get_input::<String>().lines().for_each(|l| {
        let matches = re.captures(l).unwrap();
        let ings = matches
            .get(1)
            .unwrap()
            .as_str()
            .split(' ')
            .map(|w| w.to_string())
            .collect::<HashSet<String>>();
        let allergens = matches
            .get(2)
            .unwrap()
            .as_str()
            .split(", ")
            .map(|w| w.to_string())
            .collect::<HashSet<String>>();

        ings.iter()
            .for_each(|s| *ing_freq.entry(s.clone()).or_insert(0) += 1);

        for allergen in allergens {
            match allerg_to_ing.get(&allergen) {
                Some(cur) => {
                    let cur = cur.to_owned();
                    allerg_to_ing.insert(allergen, cur.intersection(&ings).cloned().collect())
                }
                None => allerg_to_ing.insert(allergen, ings.clone()),
            };
        }
    });

    println!("Part 1: {}", part1(&allerg_to_ing, &ing_freq));
    println!("Part 2: {}", part2(allerg_to_ing));
}
