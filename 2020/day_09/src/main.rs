use std::collections::{BTreeMap, BTreeSet};

fn add_number(table: &mut BTreeMap<u64, BTreeSet<u64>>, inputs: &[u64], index: usize, n: u64) {
    let si = (index as i32 - 25).max(0) as usize;
    let mut sums = BTreeSet::new();
    for adder in inputs[si..index].iter() {
        sums.insert(adder + n);
    }
    table.insert(index as u64, sums);
}

fn contains(table: &BTreeMap<u64, BTreeSet<u64>>, i: usize, n: u64) -> bool {
    let mut si = i as i32 - 25;
    if si < 0 {
        si = 0;
    }
    let si = si as usize;
    let mut result = false;
    for index in si..i {
        if table.get(&(index as u64)).unwrap().contains(&n) {
            result = true;
            break
        }
    }
    result
}

fn main() {
    let inputs: Vec<u64> = input_reader::get_inputs();

    let mut table: BTreeMap<u64, BTreeSet<u64>> = BTreeMap::new();

    let mut index: usize = 0;
    for num in inputs[0..25].iter() {
        add_number(&mut table, &inputs, index, *num);
        index += 1;
    }

    let mut goal: u64 = 0;
    for num in inputs[25..].iter() {
        if !contains(&table, index, *num) {
            goal = *num;
            println!("Part 1: {}", num);
            break
        }
        add_number(&mut table, &inputs, index, *num);
        index += 1;
    }

    let inputs: Vec<u64> = input_reader::get_inputs();
    let mut sum_range = 0..0;
    for i in 0..inputs.len() {
        let mut sum: u64 = 0;
        let mut steps = 0;
        for n2 in inputs[i..].iter() {
            sum += n2;
            steps += 1;
            //println!("{}: {} - {} < {}", sum, i, i+steps, goal);
            if sum == goal {
                sum_range = i..i+steps;
                break
            } else if sum > goal {
                break
            }
        }
        if sum_range.len() > 0 {
            break
        }
    }

    let mut range: Vec<u64> = sum_range.map(|i| *inputs.get(i).unwrap()).collect();
    range.sort();

    let first = range.iter().next().unwrap();
    let last = range.iter().last().unwrap();
    println!("Part 2: {}", first + last);
}
