use std::{str::FromStr, fs};

// Re-exports
pub use regex::*;
pub use text_io::*;

pub fn get_inputs_split<T>(sep: &str) -> Vec<T>
where
    T: FromStr,
    <T as std::str::FromStr>::Err: std::fmt::Debug,
{
    fs::read_to_string("input.txt")
        .expect("Unable to read input.txt")
        .trim()
        .split(sep)
        .map(|s| s.parse::<T>().expect("Unable to convert value"))
        .collect()
}

pub fn get_inputs<T>() -> Vec<T>
where 
    T: FromStr,
    <T as std::str::FromStr>::Err: std::fmt::Debug,
{
    get_inputs_split("\n")
}

pub fn get_input<T>() -> T
where 
    T: FromStr,
    <T as std::str::FromStr>::Err: std::fmt::Debug,
{
    fs::read_to_string("input.txt")
        .expect("Unable to read input.txt")
        .parse::<T>()
        .expect("Unable to parse value from input.txt")
}

#[cfg(test)]
mod tests {
    use std::{fs, panic};

    use super::*;

    #[test]
    fn test_get_lines() {
        run_test(|| {
            let lines: Vec<String> = get_inputs();
            for (index, line) in lines.iter().enumerate() {
                assert_eq!(line, &format!("{}", index+1));
            }
        });
    }

    #[test]
    fn test_get_integers() {
        run_test(|| {
            let lines: Vec<i32> = get_inputs();
            for (index, num) in lines.iter().enumerate() {
                assert_eq!(*num, index as i32 + 1);
            }
        });
    }

    #[test]
    fn test_get_unsigned() {
        run_test(|| {
            let lines: Vec<u32> = get_inputs();
            for (index, num) in lines.iter().enumerate() {
                assert_eq!(*num, index as u32 + 1);
            }
        });
    }

    #[test]
    fn test_get_input() {
        run_test(|| {
            let input: String = get_input();
            assert_eq!(input, "1\n2\n3\n4\n5");
        });
    }

    fn setup() {
        let numbers = vec![1, 2, 3, 4, 5];
        let strings: Vec<String> = numbers.iter().map(|n| n.to_string()).collect();
        let bytes: Vec<u8> = strings.join("\n").bytes().collect();
        fs::write("input.txt", &bytes).unwrap();
    }

    fn teardown() {
        fs::remove_file("input.txt").unwrap();
    }

    fn run_test<T>(test: T) -> ()
        where T: FnOnce() -> () + panic::UnwindSafe
    {
        setup();
        let result = panic::catch_unwind(|| test());
        teardown();
        assert!(result.is_ok());

    }
}
