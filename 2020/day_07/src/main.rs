use std::collections::BTreeMap;

type Rules = BTreeMap<String, BTreeMap<String, u32>>;

fn get_rules(lines: &[String]) -> Rules {
    let mut rules: Rules = BTreeMap::new();
    for line in lines {
        let mut split = line.splitn(2, " bags contain ");
        let (bag, contains_str): (&str, &str) = (split.next().unwrap(), split.next().unwrap());
        let mut inner = BTreeMap::new();
        if !contains_str.starts_with("no other") {
            for contains in contains_str.split(", ") {
                let contained: Vec<&str> = contains.split(" ").collect();
                let count: u32 = contained[0].parse().unwrap();
                let color = format!("{} {}", contained[1], contained[2]);
                inner.insert(color, count);
            }
        }
        rules.insert(bag.to_string(), inner);
    }
    rules
}

fn count_gold_bags(key: &str, rules: &Rules) -> bool {
    if key == "shiny gold" {
        true
    } else {
        let rule = rules.get(key).unwrap();
        let mut result = false;
        for bag in rule.keys() {
            result = count_gold_bags(bag, rules);
            if result {
                break
            }
        }
        result
    }
}

fn count_contained_bags(key: &str, rules: &Rules) -> u32 {
    let contained = rules.get(key).unwrap();
    if contained.is_empty() {
        1
    } else {
        let mut sum = 1;
        for (color, count) in contained {
            sum += count * count_contained_bags(color, rules);
        }
        sum
    }
}

fn main() {
    let lines: Vec<String> = input_reader::get_inputs();
    let rules = get_rules(&lines);

    let mut sum = 0;
    for bag in rules.keys() {
        if bag != "shiny gold" {
            if count_gold_bags(bag, &rules) {
                sum += 1;
            }
        }
    }
    println!("Part 1: {}", sum);
    println!("Part 2: {}", count_contained_bags("shiny gold", &rules) - 1);
}
