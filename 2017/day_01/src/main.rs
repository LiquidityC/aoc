use std::fs;

fn main() {
    let input = fs::read_to_string("input.txt").expect("Unable to read file");
    let input = input.trim();

    let shifted_once = shift_str(&input, 1);
    println!("Part 1: {}", sum_duplicates(&input, &shifted_once));

    let shifted_half = shift_str(&input, (input.len() as u32)/2);
    println!("Part 2: {}", sum_duplicates(&input, &shifted_half));
}

fn shift_str(string: &str, count: u32) -> String {
    let mut chars = string.chars();
    let first_char = chars.next().unwrap();
    let mut shifted = String::from(chars.as_str());
    shifted.push(first_char);
    if count - 1 == 0 {
        shifted
    } else {
        shift_str(shifted.as_str(), count - 1)
    }
}

fn sum_duplicates<'a>(str1: &str, str2: &str) -> u32 {
    let mut sum: u32 = 0;
    for pair in str1.chars().zip(str2.chars()) {
        if pair.0 == pair.1 {
            sum = sum + pair.0.to_digit(10).unwrap();
        }
    }
    sum
}
