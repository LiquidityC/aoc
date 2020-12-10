fn find_arrangements(adaptors: &[u32]) -> u64 {
    let mut counts = vec![0; adaptors.len()];

    let clen = counts.len();
    counts[clen - 1] = 1;
    for i in (0..adaptors.len() - 1).rev() {
        let mut s = 0;
        for (j, val) in adaptors[i + 1..].iter().enumerate() {
            if val - adaptors[i] <= 3 {
                s += counts[i + j + 1]
            } else {
                break;
            }
        }
        counts[i] = s;
    }
    counts[0]
}

fn main() {
    let mut adaptors: Vec<u32> = input_reader::get_inputs();
    let device = adaptors.iter().max().unwrap() + 3;
    adaptors.push(0);
    adaptors.push(device);
    adaptors.sort_unstable();

    let mut one_diff_count = 0;
    let mut three_diff_count = 0;
    for win in adaptors.windows(2) {
        let min = win[0];
        let max = win[1];
        match max - min {
            1 => one_diff_count += 1,
            3 => three_diff_count += 1,
            _ => {}
        }
    }
    println!("Part 1: {}", one_diff_count * three_diff_count);
    println!("Part 2: {}", find_arrangements(&adaptors));
}
