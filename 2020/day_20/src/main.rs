use std::{collections::BTreeMap, iter::FromIterator};

#[derive(Debug, Clone)]
struct Image {
    inner: Vec<Vec<char>>,
}

impl Image {
    fn new() -> Self {
        Self { inner: vec![] }
    }

    fn top(&self) -> Vec<char> {
        self.iter().next().unwrap().clone()
    }

    fn bottom(&self) -> Vec<char> {
        self.iter().last().unwrap().clone()
    }

    fn left(&self) -> Vec<char> {
        self.iter().map(|r| *r.iter().next().unwrap()).collect()
    }

    fn right(&self) -> Vec<char> {
        self.iter().map(|r| *r.iter().last().unwrap()).collect()
    }

    fn flip(&mut self) {
        self.inner.reverse();
    }

    fn rotate(&mut self) {
        let len = self.inner.len();
        let mut new_inner: Vec<Vec<char>> = vec![];
        for x in (0..len).rev() {
            let mut r = vec![];
            for y in 0..len {
                r.push(self.inner[y][x])
            }
            new_inner.push(r);
        }
        self.inner = new_inner;
    }

    fn edges(&self) -> Vec<Vec<char>> {
        vec![self.top(), self.right(), self.bottom(), self.left()]
    }
}

impl From<&Grid> for Image {
    fn from(g: &Grid) -> Self {
        let mut inner: Vec<Vec<char>> = vec![vec![]; 8 * 12];
        for (ri, r) in g.iter().enumerate() {
            for t in r.iter() {
                let img = &t.image;
                for (ii, s) in img[1..9].iter().enumerate() {
                    let index = ii + ri * 8;
                    inner.get_mut(index).unwrap().append(&mut s[1..9].to_vec());
                }
            }
        }
        assert_eq!(inner.len(), 8 * 12);
        assert_eq!(inner[0].len(), 8 * 12);
        Self { inner }
    }
}

impl std::fmt::Display for Image {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for r in self.iter() {
            writeln!(f, "{}", r.iter().collect::<String>())?;
        }
        Ok(())
    }
}

impl std::ops::Deref for Image {
    type Target = Vec<Vec<char>>;

    fn deref(&self) -> &Self::Target {
        &self.inner
    }
}

impl std::ops::DerefMut for Image {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.inner
    }
}

type Direction = (i32, i32);
const RIGHT: (i32, i32) = (1, 0);
const LEFT: (i32, i32) = (-1, 0);
const UP: (i32, i32) = (0, -1);
const DOWN: (i32, i32) = (0, 1);

type NeighborList = BTreeMap<Direction, Vec<u64>>;

#[derive(Debug, Clone)]
struct Tile {
    id: u64,
    image: Image,
    neighbors: NeighborList,
}

impl Tile {
    fn has_edge(&self, edge: &[char]) -> bool {
        let edges = self.image.edges();
        let rev = edge.iter().rev().cloned().collect();
        edges.contains(&edge.to_vec()) || edges.contains(&rev)
    }

    fn rotate(&mut self) {
        let old_left = self.neighbors.get(&LEFT).unwrap().clone();
        self.neighbors
            .insert(LEFT, self.neighbors.get(&UP).unwrap().clone());
        self.neighbors
            .insert(UP, self.neighbors.get(&RIGHT).unwrap().clone());
        self.neighbors
            .insert(RIGHT, self.neighbors.get(&DOWN).unwrap().clone());
        self.neighbors.insert(DOWN, old_left);
        self.image.rotate();
    }

    fn flip(&mut self) {
        let old_top = self.neighbors.get(&UP).unwrap().clone();
        self.neighbors
            .insert(UP, self.neighbors.get(&DOWN).unwrap().clone());
        self.neighbors.insert(DOWN, old_top);
        self.image.flip();
    }

    fn populate_neighbors(&mut self, tiles: &[Tile]) {
        self.neighbors.insert(UP, vec![]);
        self.neighbors.insert(DOWN, vec![]);
        self.neighbors.insert(LEFT, vec![]);
        self.neighbors.insert(RIGHT, vec![]);

        let top = self.image.top();
        let bottom = self.image.bottom();
        let left = self.image.left();
        let right = self.image.right();
        for t in tiles {
            if self.id == t.id {
                continue;
            }
            if t.has_edge(&top) {
                self.neighbors.get_mut(&UP).unwrap().push(t.id);
            }
            if t.has_edge(&bottom) {
                self.neighbors.get_mut(&DOWN).unwrap().push(t.id);
            }
            if t.has_edge(&left) {
                self.neighbors.get_mut(&LEFT).unwrap().push(t.id);
            }
            if t.has_edge(&right) {
                self.neighbors.get_mut(&RIGHT).unwrap().push(t.id);
            }
        }
    }

    fn neighbored_sides(&self) -> usize {
        self.neighbors.values().filter(|v| !v.is_empty()).count()
    }

    fn neighbors(&self, dir: Direction) -> Vec<u64> {
        self.neighbors.get(&dir).unwrap().clone()
    }

    fn states(&self) -> Vec<Tile> {
        let mut state = self.clone();
        let mut results = vec![];
        for flip in &[false, true] {
            if *flip {
                let ntop: Vec<u64> = state.neighbors(UP);
                let nbot: Vec<u64> = state.neighbors(DOWN);
                state.flip();
                assert_eq!(ntop, state.neighbors(DOWN));
                assert_eq!(nbot, state.neighbors(UP));
            }
            for _ in 0..4 {
                let ntop: Vec<u64> = state.neighbors(UP);
                state.rotate();
                assert_eq!(ntop, state.neighbors(LEFT));
                results.push(state.clone())
            }
        }
        assert_eq!(results.len(), 8);
        results
    }
}

impl std::fmt::Display for Tile {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(f, "id: {}", self.id)?;
        write!(f, "{}", self.image)
    }
}

impl std::str::FromStr for Tile {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut line = s.lines();
        let id: u64 = {
            let num = line.next().unwrap().split(' ').last().unwrap();
            let num = num.replace(':', "");
            num.parse::<u64>().unwrap()
        };

        let mut image = Image::new();
        for l in line {
            image.push(Vec::from_iter(l.chars()));
        }
        Ok(Self {
            id,
            image,
            neighbors: NeighborList::new(),
        })
    }
}

type Row = Vec<Tile>;
type Grid = Vec<Row>;

fn build_image(tiles: &Tiles) -> Grid {
    let corner = tiles.values().find(|t| t.neighbored_sides() == 2).unwrap();

    let mut grid = Grid::new();
    let mut row = Row::new();
    let corner = corner
        .states()
        .iter()
        .find(|c| c.neighbors(LEFT).is_empty() && c.neighbors(UP).is_empty())
        .unwrap()
        .clone();

    row.push(corner);
    while row.len() < 12 {
        let last = row.iter().last().unwrap();
        let neighbors = last.neighbors(RIGHT);
        'find: for id in neighbors {
            let n = tiles.get(&id).unwrap();
            for t in n.states() {
                if t.image.left() == last.image.right() && t.neighbors(UP).is_empty() {
                    row.push(t);
                    break 'find;
                }
            }
        }
    }
    grid.push(row);

    while grid.len() < 12 {
        let last_row = grid.last().unwrap();
        let mut row = Row::new();
        while row.is_empty() {
            let last = last_row.first().unwrap();
            let neighbors = last.neighbors(DOWN);
            'first: for id in neighbors {
                let n = tiles.get(&id).unwrap();
                for t in n.states() {
                    if t.image.top() == last.image.bottom() && t.neighbors(LEFT).is_empty() {
                        row.push(t);
                        break 'first;
                    }
                }
            }
        }
        while row.len() < 12 {
            let above = &last_row[row.len()];
            let last = row.last().unwrap();
            let neighbors = last.neighbors(RIGHT);
            'next: for id in neighbors {
                let n = tiles.get(&id).unwrap();
                for t in n.states() {
                    if t.image.left() == last.image.right() && t.image.top() == above.image.bottom()
                    {
                        row.push(t);
                        break 'next;
                    }
                }
            }
        }
        grid.push(row);
    }
    grid
}

type Tiles = BTreeMap<u64, Tile>;
fn main() {
    let tile_list: Vec<Tile> = input_reader::get_inputs_split("\n\n");
    let mut tiles: Tiles = tile_list.iter().map(|t| (t.id, t.clone())).collect();
    let dim = (tiles.len() as f64).sqrt();
    tiles
        .values_mut()
        .for_each(|t| t.populate_neighbors(&tile_list));
    assert!(tiles.values().all(|t| t.neighbored_sides() >= 2));
    assert_eq!(
        tiles.values().filter(|t| t.neighbored_sides() == 3).count(),
        (dim as usize - 2) * 4
    );
    assert_eq!(
        tiles.values().filter(|t| t.neighbored_sides() == 2).count(),
        4
    );

    let grid = build_image(&tiles);
    let mut grid_valid = 1;
    grid_valid *= grid.first().unwrap().first().unwrap().id;
    grid_valid *= grid.first().unwrap().last().unwrap().id;
    grid_valid *= grid.last().unwrap().first().unwrap().id;
    grid_valid *= grid.last().unwrap().last().unwrap().id;

    let valid = tiles
        .values()
        .filter_map(|t| {
            if t.neighbored_sides() == 2 {
                Some(t.id)
            } else {
                None
            }
        })
        .product::<u64>();
    assert_eq!(valid, grid_valid);
    println!("Part 1: {}", valid);

    let mut image = Image::from(&grid);
    let seamonster: Vec<String> = r#"                  # 
#    ##    ##    ###
 #  #  #  #  #  #   "#
        .split('\n')
        .map(|l| l.to_string())
        .collect();

    let mut count = 0;
    'monster: for flip in &[false, true] {
        if *flip {
            image.flip();
        }
        for _ in 0..4 {
            image.rotate();
            for rows in image.windows(3) {
                let r1 = rows[0].windows(20);
                let mut r2 = rows[1].windows(20);
                let mut r3 = rows[2].windows(20);

                for sub1 in r1 {
                    let sub2 = r2.next().unwrap();
                    let sub3 = r3.next().unwrap();
                    let first = vec_cmp(&seamonster[0], sub1);
                    let second = vec_cmp(&seamonster[1], sub2);
                    let third = vec_cmp(&seamonster[2], sub3);
                    if first && second && third {
                        count += 15;
                    }
                }
            }
            if count > 0 {
                break 'monster;
            }
        }
    }
    println!(
        "Part 2: {}",
        image.iter().flatten().filter(|c| **c == '#').count() - count
    );
}

fn vec_cmp(a: &str, b: &[char]) -> bool {
    a.chars()
        .zip(b.iter())
        .all(|(c1, c2)| if c1 == '#' { c1 == *c2 } else { true })
}
