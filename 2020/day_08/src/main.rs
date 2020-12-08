use computer::{ExecResult, Instruction, Processor};

fn main() {
    let mut inputs: Vec<Instruction> = input_reader::get_inputs();
    let mut comp = Processor::default();
    if let ExecResult::Cycle(result) = comp.execute(&inputs) {
        println!("Part 1: {}", result);
    }

    for i in 0..inputs.len() {
        let mut result = ExecResult::Incomplete;
        let instr = &inputs[i];
        comp.reset();
        if instr.op.as_str() == "nop" {
            inputs[i].op = "jmp".to_string();
            result = comp.execute(&inputs);
            inputs[i].op = "nop".to_string();
        } else if instr.op.as_str() == "jmp" {
            inputs[i].op = "nop".to_string();
            result = comp.execute(&inputs);
            inputs[i].op = "jmp".to_string();
        }
        if let ExecResult::Complete(val) = result {
            println!("Part 2: {}", val);
            break;
        }
    }
}
