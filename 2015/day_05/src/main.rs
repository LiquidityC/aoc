use fancy_regex::Regex;

fn validate(line: &str) -> bool {
    let illegals = vec!["ab", "cd", "pq", "xy"];

    let illegal = illegals.iter().filter(|sub| line.contains(*sub)).count() > 0;

    let vowels = line.chars().filter(|c| "aeiou".contains(*c)).count();
    let double = line.chars().collect::<Vec<char>>().windows(2).filter(|win| win[0] == win[1]).count() > 0;
    double && vowels >= 3 && !illegal
}

fn alternate_validate(line: &str) -> bool {
    let pairs = Regex::new(r"(\w\w)\w*\1").unwrap();
    let repeat = Regex::new(r"(\w)\w\1").unwrap();
    pairs.is_match(line).unwrap() && repeat.is_match(line).unwrap()
}

fn main() {
    let lines: Vec<String> = input_reader::get_inputs();
    println!("Part 1: {}", lines.iter().filter(|l| validate(l)).count());
    println!("Part 2: {}", lines.iter().filter(|l| alternate_validate(l)).count());
}
