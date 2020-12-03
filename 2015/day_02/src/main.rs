use text_io::scan;

struct Prism {
    l: u32,
    w: u32,
    h: u32,
}

impl Prism {
    fn surface(&self) -> u32 {
        2*self.l*self.w + 2*self.w*self.h + 2*self.l*self.h
    }

    fn extra(&self) -> u32 {
        let front = self.h * self.w;
        let side = self.l * self.h;
        let top = self.l * self.w;
        front.min(top).min(side)
    }

    fn ribbon(&self) -> u32 {
        let mut smallest = vec![self.w, self.h, self.l];
        smallest.sort();

        let mut it = smallest.iter();
        let s1 = it.next().unwrap();
        let s2 = it.next().unwrap();

        s1 + s1 + s2 + s2 + (self.h * self.w * self.l)
    }
}

impl std::str::FromStr for Prism {
    type Err = Box<dyn std::error::Error>;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let w: u32;
        let h: u32;
        let l: u32;
        scan!(s.bytes() => "{}x{}x{}", w, h, l);
        Ok(Self { l, w, h })
    }
}

fn part1(input: &Vec<Prism>) {
    let area: u32 = input.iter().map(|p| p.surface() + p.extra()).sum();
    let ribbon: u32 = input.iter().map(|p| p.ribbon()).sum();
    println!("Part 1: {}", area);
    println!("Part 2: {}", ribbon);
}

fn main() {
    let input: Vec<Prism> = input_reader::get_inputs();
    part1(&input);
}
