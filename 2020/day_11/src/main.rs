use std::collections::VecDeque;

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
enum Type {
    Occupied,
    Empty,
    Floor,
}

type Seats = VecDeque<Vec<Type>>;

fn cycle(seats: &mut Seats) -> bool {
    let dirs: Vec<(i32, i32)> = vec![
        (1, 0),
        (-1, 0),
        (0, 1),
        (0, -1),
        (1, 1),
        (-1, 1),
        (1, -1),
        (-1, -1),
    ];
    let mut empty_seats: Vec<(usize, usize)> = vec![];
    let mut occupied_seats: Vec<(usize, usize)> = vec![];
    for y in 1..seats.len() - 1 {
        let row = &seats[y];
        for x in 1..row.len() - 1 {
            let seat = seats[y][x];
            if seat != Type::Floor {
                if seat == Type::Empty {
                    let empty_count = dirs
                        .iter()
                        .map(|(dy, dx)| seats[(y as i32 + dy) as usize][(x as i32 + dx) as usize])
                        .filter(|s| s == &Type::Empty || s == &Type::Floor)
                        .count();
                    if empty_count == 8 {
                        occupied_seats.push((y, x));
                    }
                } else if seat == Type::Occupied {
                    let occupied_count = dirs
                        .iter()
                        .map(|(dy, dx)| seats[(y as i32 + dy) as usize][(x as i32 + dx) as usize])
                        .filter(|s| s == &Type::Occupied)
                        .count();
                    if occupied_count >= 4 {
                        empty_seats.push((y, x));
                    }
                }
            }
        }
    }

    for (dy, dx) in occupied_seats.iter() {
        seats[*dy][*dx] = Type::Occupied;
    }

    for (dy, dx) in empty_seats.iter() {
        seats[*dy][*dx] = Type::Empty;
    }

    !occupied_seats.is_empty() || !empty_seats.is_empty()
}

fn look(seats: &Seats, origin: (usize, usize), dir: (i32, i32)) -> Type {
    let mut result = Type::Floor;
    let height = seats.len();
    let width = seats[0].len();
    let mut dy: i32 = origin.0 as i32;
    let mut dx: i32 = origin.1 as i32;

    loop {
        dy += dir.0;
        dx += dir.1;

        if !(0..height).contains(&(dy as usize)) || !(0..width).contains(&(dx as usize)) {
            break;
        }

        result = seats[dy as usize][dx as usize];
        if result != Type::Floor {
            break;
        }
    }

    if result == Type::Floor {
        result = Type::Empty
    }
    result
}

fn cycle2(seats: &mut Seats) -> bool {
    let dirs: Vec<(i32, i32)> = vec![
        (1, 0),
        (-1, 0),
        (0, 1),
        (0, -1),
        (1, 1),
        (-1, 1),
        (1, -1),
        (-1, -1),
    ];
    let mut empty_seats: Vec<(usize, usize)> = vec![];
    let mut occupied_seats: Vec<(usize, usize)> = vec![];
    for y in 1..seats.len() - 1 {
        let row = &seats[y];
        for x in 1..row.len() - 1 {
            let seat = seats[y][x];
            if seat != Type::Floor {
                if seat == Type::Empty {
                    let empty_count = dirs
                        .iter()
                        .filter(|dir| look(&seats, (y, x), **dir) == Type::Empty)
                        .count();
                    if empty_count == 8 {
                        occupied_seats.push((y, x));
                    }
                } else if seat == Type::Occupied {
                    let occupied_count = dirs
                        .iter()
                        .filter(|dir| look(&seats, (y, x), **dir) == Type::Occupied)
                        .count();
                    if occupied_count >= 5 {
                        empty_seats.push((y, x));
                    }
                }
            }
        }
    }

    for (dy, dx) in occupied_seats.iter() {
        seats[*dy][*dx] = Type::Occupied;
    }

    for (dy, dx) in empty_seats.iter() {
        seats[*dy][*dx] = Type::Empty;
    }

    !occupied_seats.is_empty() || !empty_seats.is_empty()
}

fn main() {
    let inputs: Vec<String> = input_reader::get_inputs();

    let mut seats: Seats = VecDeque::new();
    for l in inputs {
        let mut row = vec![];
        row.push(Type::Floor);
        for c in l.chars() {
            row.push(match c {
                'L' => Type::Empty,
                '#' => Type::Occupied,
                _ => Type::Floor,
            });
        }
        row.push(Type::Floor);
        seats.push_back(row);
    }

    let row_len = seats[0].len();
    seats.push_back(vec![Type::Floor; row_len]);
    seats.push_front(vec![Type::Floor; row_len]);

    let mut seats2 = seats.clone();

    loop {
        if !cycle(&mut seats) {
            break;
        }
    }
    println!(
        "Part 1: {}",
        seats
            .iter()
            .flatten()
            .filter(|s| *s == &Type::Occupied)
            .count()
    );

    loop {
        if !cycle2(&mut seats2) {
            break;
        }
    }
    println!(
        "Part 2: {}",
        seats2
            .iter()
            .flatten()
            .filter(|s| *s == &Type::Occupied)
            .count()
    );
}
