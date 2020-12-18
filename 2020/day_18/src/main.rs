#[derive(Copy, Clone, PartialEq, Eq)]
enum Op {
    Mul,
    Add,
    None,
}

fn calc(op: Op, a: i64, b: i64) -> i64 {
    if a == 0 {
        b
    } else {
        match op {
            Op::Add => a + b,
            Op::Mul => a * b,
            _ => panic!("Bad value"),
        }
    }
}

fn solve(p: &str, index: &mut usize) -> i64 {
    let mut op = Op::None;
    let mut sum: i64 = 0;
    while *index < p.len() {
        let c = p.chars().nth(*index).unwrap();
        if c == ')' {
            break;
        }
        if c == '(' {
            *index += 1;
            sum = calc(op, sum, solve(p, index));
            *index += 1;
            continue;
        }
        if c.is_digit(10) {
            sum = calc(op, sum, c.to_digit(10).unwrap() as i64);
            *index += 1;
            continue;
        }
        match c {
            '+' => op = Op::Add,
            '*' => op = Op::Mul,
            _ => unreachable!("This wasn't good"),
        }
        *index += 1;
    }
    sum
}

fn solve2(p: &str, index: &mut usize) -> i64 {
    let mut op = Op::None;
    let mut products: Vec<i64> = vec![];
    let mut sum: i64 = 0;
    while *index < p.len() {
        let c = p.chars().nth(*index).unwrap();
        if c == ')' {
            break;
        }
        if c == '(' {
            *index += 1;
            match op {
                Op::Add => sum = calc(op, sum, solve2(p, index)),
                Op::Mul => {
                    products.push(sum);
                    sum = solve2(p, index);
                }
                Op::None => sum = solve2(p, index),
            }
            *index += 1;
            continue;
        }
        if c.is_digit(10) {
            let num = c.to_digit(10).unwrap() as i64;
            match op {
                Op::Mul => {
                    products.push(sum);
                    sum = num;
                }
                Op::Add => sum = calc(op, sum, num),
                Op::None => sum = num,
            }
            *index += 1;
            continue;
        }
        match c {
            '+' => op = Op::Add,
            '*' => op = Op::Mul,
            _ => unreachable!("This wasn't good"),
        }
        *index += 1;
    }
    products.push(sum);
    products.iter().product()
}

fn main() {
    let mut lines: Vec<String> = input_reader::get_inputs();
    lines.iter_mut().for_each(|l| *l = l.replace(" ", ""));

    println!(
        "Part 1: {}",
        lines.iter().map(|l| solve(&l, &mut 0)).sum::<i64>()
    );
    println!(
        "Part 2: {}",
        lines.iter().map(|l| solve2(&l, &mut 0)).sum::<i64>()
    );
}
