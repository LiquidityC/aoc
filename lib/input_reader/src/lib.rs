use std::{str::FromStr, fs};

pub fn get_input_lines() -> Vec<String> {
    fs::read_to_string("input.txt")
        .expect("Unable to read input.txt")
        .trim()
        .split("\n")
        .map(|s| s.to_string())
        .collect()
}

pub fn get_input_number<T: FromStr>() -> T where <T as std::str::FromStr>::Err: std::fmt::Debug  {
    fs::read_to_string("input.txt")
        .expect("Unable to read input.txt")
        .parse()
        .expect("Unable to parse number from input.txt")
}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }
}
