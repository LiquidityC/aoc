fn main() {
    let input: String = input_reader::get_input();

    let mut num: u32 = 0;
    let mut first: u32 = 0;
    let mut second: u32 = 0;
    loop {
        num += 1;
        let key = format!("{}{}", input, num);
        let digest = md5::compute(key);

        if first == 0 && format!("{:x}", digest).starts_with("00000") {
            first = num;
        }
        if second == 0 && format!("{:x}", digest).starts_with("000000") {
            second = num;
        }
        if first > 0 && second > 0 {
            break;
        }
    }
    println!("Part 1: {}", first);
    println!("Part 2: {}", second);
}
