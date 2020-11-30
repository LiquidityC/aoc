use input_reader::get_input_number;

fn main() {
    calculat_distance_for(get_input_number());
}

fn calculat_distance_for(number: u32) {
    let dim = find_square_dim(number);
    let center_dist = find_shortest_distance_from_center(dim, number);
    println!("Part 1: {}", center_dist + (dim-1)/2);
}

fn find_square_dim(number: u32) -> u32 {
    let mut dim = 1u32;
    loop {
        if dim*dim > number {
            return dim
        }
        dim += 2;
    }
}

fn find_shortest_distance_from_center(dim: u32, num: u32) -> u32 {
    let max_num = dim*dim;
    let half_dist = (dim-1)/2;
    let bottom_center = max_num - half_dist;
    let left_center = bottom_center - half_dist*2;
    let top_center = left_center - half_dist*2;
    let right_center = top_center - half_dist*2;

    let options = vec![bottom_center, left_center, top_center, right_center];
    let mut distance = dim as i32;
    for opt in options {
        distance = distance.min((opt as i32 - num as i32).abs());
    }
    distance as u32
}
