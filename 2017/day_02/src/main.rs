use std::fs;

fn main() {
    let input = fs::read_to_string("input.txt")
        .expect("Unable to read file");

    let lines = input.trim().split("\n").collect();
    println!("Part 1: {}", calculate_checksum(&lines));
    println!("Part 2: {}",
        calculate_even_divisor_checksum(&lines));
}

fn calculate_checksum(lines: &Vec<&str>) -> u32 {
    let mut checksum = 0;
    for line in lines {
        let mut largest: u32 = 0;
        let mut smallest: u32 = 10000;
        for s_num in line.split("\t") {
            let num = s_num.parse().unwrap();
            largest = largest.max(num);
            smallest = smallest.min(num);
        }
        checksum = checksum + largest - smallest;
    }
    checksum
}

fn calculate_even_divisor_checksum(lines: &Vec<&str>) -> u32 {
    let mut checksum = 0u32;
    for line in lines {
        for (cur_i, s_num) in line.split("\t").enumerate() {
            let current: f32 = s_num.parse().unwrap();
            for (other_i, s_num2) in line.split("\t").enumerate() {
                if cur_i == other_i {
                    continue
                }
                let other: f32 = s_num2.parse().unwrap();
                let part = current / other;
                if part.floor() == part {
                    checksum = checksum + part as u32;
                }
                
            }
        }
    }
    checksum
}
