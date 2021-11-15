fn find(public_key: usize) -> usize {
    let mut loop_size = 1;
    let mut value = 1;
    loop {
        value *= 7;
        value = value % 20201227;
        if value == public_key {
            return loop_size;
        }
        loop_size += 1;
    }
}

fn transform(loop_size: usize, subject: usize) -> usize {
    let mut value = 1;
    for _ in 0..loop_size {
        value *= subject;
        value = value % 20201227;
    }
    return value;
}

fn main() {
    let public_keys: Vec<usize> = input_reader::get_inputs();

    let loop_sizes: Vec<usize> = public_keys
        .iter()
        .map(|&public_key| find(public_key))
        .collect();

    println!("Part 1: {}", transform(loop_sizes[0], public_keys[1]));
}
