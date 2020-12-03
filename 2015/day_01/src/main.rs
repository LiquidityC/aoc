fn part1(input: &String) {
    let mut floor = 0;
    let mut basement = 0;
    for (i, c) in input.char_indices() {
        floor += match c {
            '(' => 1,
            ')' => -1,
            c => {
                println!("Bad char: {}", c);
                0
            }
        };

        if basement == 0 && floor == -1 {
            basement = i + 1;
        }
    }

    println!("Part 1: {}", floor);
    println!("Part 2: {}", basement);
}
fn main() {
    let input: String = input_reader::get_input();
    part1(&input);
}
