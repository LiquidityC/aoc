#[derive(Copy, Clone, PartialEq, Eq)]
enum Op {
    Mul,
    Add,
    None,
}

impl Op {
    fn calc(&self, a: i64, b: i64) -> i64 {
        match self {
            Op::Add => a + b,
            Op::Mul => a * b,
            _ => b,
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
        } else if c == '(' {
            *index += 1;
            sum = op.calc(sum, solve(p, index));
        } else if c.is_digit(10) {
            sum = op.calc(sum, c.to_digit(10).unwrap() as i64);
        } else {
            match c {
                '+' => op = Op::Add,
                '*' => op = Op::Mul,
                _ => unreachable!("This wasn't good"),
            }
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
        } else if c == '(' {
            *index += 1;
            match op {
                Op::Add => sum = op.calc(sum, solve2(p, index)),
                Op::Mul => {
                    products.push(sum);
                    sum = solve2(p, index);
                }
                Op::None => sum = solve2(p, index),
            }
        } else if c.is_digit(10) {
            let num = c.to_digit(10).unwrap() as i64;
            match op {
                Op::Mul => {
                    products.push(sum);
                    sum = num;
                }
                Op::Add => sum = op.calc(sum, num),
                Op::None => sum = num,
            }
        } else {
            match c {
                '+' => op = Op::Add,
                '*' => op = Op::Mul,
                _ => unreachable!("This wasn't good"),
            }
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
