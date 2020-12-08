use computer::{Computer, ExecResult};

fn main() {
    let mut comp: Computer = input_reader::get_input();
    if let ExecResult::Cycle(result) = comp.run() {
        println!("Part 1: {}", result);
    }

    for i in 0..comp.len() {
        let mut result = ExecResult::Incomplete;
        let instr = comp[i].op.clone();
        comp.reset();
        if instr == "nop" {
            comp[i].op = "jmp".to_string();
            result = comp.run();
            comp[i].op = "nop".to_string();
        } else if instr == "jmp" {
            comp[i].op = "nop".to_string();
            result = comp.run();
            comp[i].op = "jmp".to_string();
        }
        if let ExecResult::Complete(val) = result {
            println!("Part 2: {}", val);
            break;
        }
    }
}
