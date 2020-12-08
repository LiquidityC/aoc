use std::{collections::HashSet, str::FromStr};

#[derive(Debug, Clone)]
pub struct Instruction {
    pub op: String,
    pub arg: i32,
}

impl FromStr for Instruction {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut split = s.split(' ');
        let op: String = split.next().unwrap().parse().unwrap();
        let arg: i32 = split.next().unwrap().parse().unwrap();
        Ok(Self { op, arg })
    }
}

pub enum ExecResult {
    Cycle(i32),
    Complete(i32),
    Invalid(i32),
    Incomplete,
}

#[derive(Default)]
pub struct Processor {
    accumulator: i32,
    iptr: i32,
}

impl Processor {
    pub fn reset(&mut self) {
        self.accumulator = 0;
        self.iptr = 0;
    }

    pub fn execute(&mut self, instructions: &[Instruction]) -> ExecResult {
        let mut executed: HashSet<i32> = HashSet::new();
        let result: ExecResult;

        loop {
            if executed.contains(&self.iptr) {
                result = ExecResult::Cycle(self.accumulator);
                break;
            } else {
                executed.insert(self.iptr);
            }

            if self.iptr == instructions.len() as i32 {
                result = ExecResult::Complete(self.accumulator);
                break;
            } else if self.iptr as usize > instructions.len() || self.iptr < 0 {
                result = ExecResult::Invalid(self.accumulator);
                break;
            }

            self.iptr += match instructions.get(self.iptr as usize) {
                Some(inst) => match inst.op.as_str() {
                    "acc" => {
                        self.accumulator += inst.arg;
                        1
                    }
                    "nop" => 1,
                    "jmp" => inst.arg,
                    _ => {
                        panic!("Bad instruction");
                    }
                },
                None => {
                    panic!("Bad instruction set");
                }
            };
        }
        result
    }
}
