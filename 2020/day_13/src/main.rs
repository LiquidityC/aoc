fn part1(ts: u64, busline: &str) {
    let busses: Vec<u64> = busline.split(",").filter(|b| *b != "x").map(|b| b.parse::<u64>().expect("Invalid bus")).collect();
    let wait_times: Vec<(u64, u64)> = busses.iter().map(|t| {
        (*t, t - (ts % t))
    }).collect();
    let min_wait = wait_times.iter().min_by(|b1, b2| b1.1.cmp(&b2.1)).unwrap();
    println!("Part 1: {}", min_wait.0 * min_wait.1);
}

fn part2(busline: &str) {
    let times: Vec<(u64, u64)> = busline.split(",").enumerate().filter(|(_, b)| *b != "x").map(|(i, b)| (i as u64, b.parse::<u64>().unwrap())).collect();
    let mut stime: u64 = 0;
    let mut incr = times[0].1;

    for (offset, bus) in &times[1..] {
        loop {
            stime += incr;
            if (stime + offset) % bus == 0 {
                break
            }
        }
        incr = incr * bus;
    }

    println!("Part 2: {}", stime);
}

fn main() {
    let lines: Vec<String> = input_reader::get_inputs();
    let mut it = lines.iter();
    let ts: u64 = it.next().unwrap().parse().expect("Invalid TS");
    let busline = it.next().unwrap();
    part1(ts, &busline);
    part2(&busline);
}
