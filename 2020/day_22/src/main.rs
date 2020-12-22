use std::collections::{HashSet, VecDeque};

fn part1(mut deck1: VecDeque<u64>, mut deck2: VecDeque<u64>) -> u64 {
    while !deck1.is_empty() && !deck2.is_empty() {
        let a = deck1.pop_front().unwrap();
        let b = deck2.pop_front().unwrap();
        if a > b {
            deck1.push_back(a);
            deck1.push_back(b);
        } else {
            deck2.push_back(b);
            deck2.push_back(a);
        }
    }

    let winner = {
        if deck1.is_empty() {
            &deck2
        } else {
            &deck1
        }
    };

    winner
        .iter()
        .rev()
        .enumerate()
        .fold(0u64, |sum, (i, v)| sum + (i as u64 + 1) * v)
}

type Cache = (HashSet<VecDeque<u64>>, HashSet<VecDeque<u64>>);

fn part2(mut deck1: VecDeque<u64>, mut deck2: VecDeque<u64>) -> (u64, u8) {
    let mut hands: Cache = (HashSet::new(), HashSet::new());
    let mut winner: Option<(&VecDeque<u64>, u8)> = None;
    while !deck1.is_empty() && !deck2.is_empty() {
        if hands.0.contains(&deck1) && hands.1.contains(&deck2) {
            winner = Some((&deck1, 1));
            break;
        }
        hands.0.insert(deck1.clone());
        hands.1.insert(deck2.clone());

        let a = deck1.pop_front().unwrap();
        let b = deck2.pop_front().unwrap();

        if a as usize <= deck1.len() && b as usize <= deck2.len() {
            let sub1 = Vec::from(deck1.clone())[..a as usize].to_vec();
            let sub2 = Vec::from(deck2.clone())[..b as usize].to_vec();
            let result = part2(VecDeque::from(sub1), VecDeque::from(sub2));
            if result.1 == 1 {
                deck1.push_back(a);
                deck1.push_back(b);
            } else {
                deck2.push_back(b);
                deck2.push_back(a);
            }
        } else if a > b {
            deck1.push_back(a);
            deck1.push_back(b);
        } else {
            deck2.push_back(b);
            deck2.push_back(a);
        }
    }

    let winner = winner.unwrap_or_else(|| {
        if deck1.is_empty() {
            (&deck2, 2)
        } else {
            (&deck1, 1)
        }
    });

    (
        winner
            .0
            .iter()
            .rev()
            .enumerate()
            .fold(0u64, |sum, (i, v)| sum + (i as u64 + 1) * v),
        winner.1,
    )
}

fn main() {
    let inputs: Vec<String> = input_reader::get_inputs_split("\n\n");

    let deck1 = inputs[0]
        .lines()
        .filter(|l| l.parse::<u64>().is_ok())
        .map(|n| n.parse::<u64>().unwrap())
        .collect::<VecDeque<u64>>();
    let deck2 = inputs[1]
        .lines()
        .filter(|l| l.parse::<u64>().is_ok())
        .map(|n| n.parse::<u64>().unwrap())
        .collect::<VecDeque<u64>>();

    println!("Part 1: {}", part1(deck1.clone(), deck2.clone()));
    println!("Part 2: {}", part2(deck1, deck2).0);
}
