use std::ops::Range;

type Seats = Vec<Vec<Option<u32>>>;
type Place = (Vec<u8>, Vec<u8>);

fn binary_search(path: &[u8], range: Range<u32>) -> u32 {
    if range.is_empty() {
        range.start
    } else {
        let mid: u32 = range.end - (range.len() as u32 / 2);
        if path[0] == 0 {
            binary_search(&path[1..], range.start..mid - 1)
        } else {
            binary_search(&path[1..], mid..range.end)
        }
    }
}

fn solve(places: &[Place]) {
    let mut seats: Seats = vec![vec![None; 8]; 128];
    for place in places {
        let row = binary_search(&place.0, 0..127);
        let col = binary_search(&place.1, 0..7);
        seats[row as usize][col as usize] = Some(row * 8 + col);
    }
    println!(
        "Part 1: {}",
        seats
            .iter()
            .flatten()
            .filter(|o| o.is_some())
            .map(|o| o.unwrap())
            .max()
            .unwrap()
    );

    let all_seats: Vec<&Option<u32>> = seats.iter().flatten().collect();
    for seat in all_seats.windows(3) {
        if seat[1].is_none() && seat[0].is_some() && seat[2].is_some() {
            println!("Part 2: {}", seat[0].unwrap() + 1);
            break;
        }
    }
}

fn main() {
    let input: Vec<String> = input_reader::get_inputs();
    let mut places: Vec<Place> = vec![];
    for line in input {
        let first: Vec<u8> = line[0..7]
            .chars()
            .map(|c| match c {
                'F' => 0,
                _ => 1,
            })
            .collect();
        let second: Vec<u8> = line[7..]
            .chars()
            .map(|c| match c {
                'L' => 0,
                _ => 1,
            })
            .collect();
        places.push((first, second));
    }

    solve(&places);
}

#[cfg(test)]
mod test {

    use super::*;

    #[test]
    fn test_binary_search() {
        assert_eq!(binary_search(&[0, 1, 0, 1, 1, 0, 0], 0..127), 44);
    }
}
