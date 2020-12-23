struct Cups {
    next: Vec<usize>,
    start: usize,
}

impl From<&Vec<usize>> for Cups {
    fn from(numbers: &Vec<usize>) -> Self {
        let mut next = vec![0; numbers.len() + 1];
        for i in 0..numbers.len() {
            next[numbers[i]] = numbers[(i + 1) % numbers.len()];
        }
        Self {
            next,
            start: numbers[0],
        }
    }
}

fn do_move(cups: &mut Cups, current: &mut usize) {
    let max = cups.next.len() - 1;
    let mut taken = [0, 0, 0];

    let mut next_val = *current;
    for t in taken.iter_mut() {
        next_val = cups.next[next_val];
        *t = next_val;
    }

    {
        let nnext = cups.next[cups.next[cups.next[cups.next[*current]]]];
        cups.next[*current] = nnext;
    }

    let mut dest = *current;
    loop {
        dest = if dest < 2 { max } else { dest - 1 };
        if !taken.contains(&dest) {
            break;
        }
    }

    let prev = cups.next[dest];
    cups.next[dest] = taken[0];
    cups.next[taken[2]] = prev;

    *current = cups.next[*current];
}

fn play(rounds: u32, cups: &mut Cups) {
    let mut current = cups.start;
    for _ in 0..rounds {
        do_move(cups, &mut current);
    }
}

fn part1(mut cups: Cups) {
    play(100, &mut cups);

    let mut curr = 1;
    let mut output = String::new();
    loop {
        curr = cups.next[curr];
        if curr == 1 {
            break;
        }
        output.push_str(curr.to_string().as_str());
    }
    println!("Part 1: {}", output);
}

fn part2(mut cups: Cups) {
    play(10_000_000, &mut cups);

    let n0 = cups.next[1];
    let n1 = cups.next[n0];
    println!("Part 2: {}", n0 * n1);
}

fn main() {
    let input: String = input_reader::get_input();
    let input = input.trim();
    let mut numbers: Vec<usize> = input
        .chars()
        .map(|c| c.to_digit(10).unwrap() as usize)
        .collect();
    part1(Cups::from(&numbers));
    let start = 1 + numbers.iter().max().copied().unwrap();
    for n in start..=1_000_000 {
        numbers.push(n);
    }
    part2(Cups::from(&numbers));
}
