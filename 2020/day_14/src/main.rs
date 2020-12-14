use std::collections::BTreeMap;

fn bits_to_num(bits: &str) -> u64 {
    bits.chars()
        .rev()
        .enumerate()
        .map(|(i, c)| if c == '1' { 2u64.pow(i as u32) } else { 0 })
        .sum()
}

fn merge(mask: &str, value: &u64) -> u64 {
    let val_str = format!("{:0>36b}", value);
    let merge_str: String = mask
        .chars()
        .zip(val_str.chars())
        .map(|(a, b)| if a == 'X' { b } else { a })
        .collect();
    bits_to_num(&merge_str)
}

fn float_merge(mask: &str, value: &u64) -> Vec<u64> {
    let val_str = format!("{:0>36b}", value);
    let results: Vec<String> =
        mask.chars()
            .zip(val_str.chars())
            .fold(Vec::<String>::new(), |mut results, (a, b)| {
                if results.is_empty() {
                    if a == 'X' {
                        results.push("0".to_string());
                        results.push("1".to_string());
                    } else if a == '1' {
                        results.push(a.to_string());
                    } else {
                        results.push(b.to_string());
                    }
                } else if a == 'X' {
                    let mut clone = results.clone();
                    results.iter_mut().for_each(|s| s.push('0'));
                    clone.iter_mut().for_each(|s| s.push('1'));
                    results.append(&mut clone);
                } else if a == '1' {
                    results.iter_mut().for_each(|s| s.push(a));
                } else {
                    results.iter_mut().for_each(|s| s.push(b));
                }
                results
            });
    results.iter().map(|s| bits_to_num(s)).collect()
}

fn main() {
    let inputs: Vec<String> = input_reader::get_inputs();
    let mut numbers_v1: BTreeMap<u64, u64> = BTreeMap::new();
    let mut numbers_v2: BTreeMap<u64, u64> = BTreeMap::new();
    let mut mask: String = "".to_string();
    let mut addr: u64;
    let mut value: u64;
    for l in inputs {
        if l.starts_with("mask") {
            mask = l
                .split(" = ")
                .last()
                .expect("Failed to parse mask")
                .to_string();
            continue;
        }

        text_io::scan!(l.bytes() => "mem[{}] = {}", addr, value);
        numbers_v1.insert(addr, merge(&mask, &value));
        for a in float_merge(&mask, &addr) {
            numbers_v2.insert(a, value);
        }
    }
    println!("Part 1: {}", numbers_v1.values().sum::<u64>());
    println!("Part 2: {}", numbers_v2.values().sum::<u64>());
}
