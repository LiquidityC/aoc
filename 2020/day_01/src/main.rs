fn part1(input: &Vec<u32>) {
    for (i1, n1) in input.iter().enumerate() {
        for n2 in &input[i1..] {
            if n1 + n2 == 2020 {
                println!("Part1: {} * {} = {}", n1, n2, n1*n2);
                return;
            }
        }
    }
}

fn part2(input: &Vec<u32>) {
    for (i1, n1) in input.iter().enumerate() {
        for (i2, n2) in input[i1..].iter().enumerate() {
            for n3 in input[i2..].iter() {
                if n1 + n2 + n3 == 2020 {
                    println!("Part2: {} * {} * {} = {}", n1, n2, n3, n1*n2*n3);
                    return;
                }
            }
        }
    }
}

fn main() {
    let input = input_reader::get_inputs::<u32>();
    part1(&input);
    part2(&input);
}
