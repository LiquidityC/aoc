use std::{
    collections::HashSet,
    ops::{Deref, DerefMut},
    str::FromStr,
};

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

#[derive(Default, Debug, Clone)]
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
            if !executed.insert(self.iptr) {
                result = ExecResult::Cycle(self.accumulator);
                break;
            }

            match &self.iptr {
                x if *x == instructions.len() as i32 => {
                    result = ExecResult::Complete(self.accumulator);
                    break;
                }
                x if *x > instructions.len() as i32 => {
                    result = ExecResult::Invalid(self.accumulator);
                    break;
                }
                x if *x < 0 => {
                    result = ExecResult::Invalid(self.accumulator);
                    break;
                }
                _ => {}
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

#[derive(Debug, Clone)]
pub struct Computer {
    pub instructions: Vec<Instruction>,
    pub proc: Processor,
}

impl Deref for Computer {
    type Target = Vec<Instruction>;

    fn deref(&self) -> &Self::Target {
        &self.instructions
    }
}

impl DerefMut for Computer {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.instructions
    }
}

impl FromStr for Computer {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let instructions: Vec<Instruction> = s
            .lines()
            .map(|l| l.parse::<Instruction>().expect("Invalid instruction"))
            .collect();
        Ok(Computer::new(&instructions))
    }
}

impl Computer {
    pub fn new(instructions: &[Instruction]) -> Self {
        Self {
            instructions: instructions.to_vec(),
            proc: Processor::default(),
        }
    }

    pub fn reset(&mut self) {
        self.proc.reset();
    }

    pub fn run(&mut self) -> ExecResult {
        self.proc.execute(&self.instructions)
    }
}
