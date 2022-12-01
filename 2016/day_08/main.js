const fs = require("fs");

function create_matrix(w, h) {
	let m = Array();
	for (let i = 0; i < h; ++i) {
		m.push(Array(w).fill("."));
	}
	return m;
}

function print_matrix(m) {
	m.forEach(r => {
		r.forEach(c => {
			if (c == "#") {
				process.stdout.write("\x1b[91;101m" + c + "\x1b[0m");
			} else {
				process.stdout.write(" ");
			}
		});
		console.log("");
	});
}

function rect(m, w, h) {
	for (let i = 0; i < h; ++i) {
		for (let j = 0; j < w; ++j) {
			m[i][j] = "#";
		}
	}
}

function rotate_row(m, r, steps) {
	let i = r;
	for (let k = 0; k < steps; ++k) {
		m[i].unshift(m[i].pop());
	}
}

function rotate_col(m, c, steps) {
	let j = c;
	for (let k = 0; k < steps; ++k) {
		let tmp = m[m.length - 1][j];
		for (let i = m.length-1; i > 0; --i) {
			m[i][j] = m[i-1][j]
		}
		m[0][j] = tmp;
	}
}

function parse_instructions(m, lines) {
	lines.forEach(l => {
		if (l == "") {
			return;
		}

		let ops = l.split(" ");
		switch (ops[0]) {
			case "rect": {
				let [w, h] = ops[1].split("x").map(x => parseInt(x));
				rect(m, w, h);
			} break;
			case "rotate": {
				let index = parseInt(ops[2].split("=")[1]);
				let steps = parseInt(ops[4]);
				if (ops[1] == "row") {
					rotate_row(m, index, steps);
				} else {
					rotate_col(m, index, steps);
				}
			} break;
			default:
				console.log("Bad op:", ops[0]);
				break;

		}
	});
}

function count_pixels(m) {
	let count = 0;
	m.forEach(r => {
		
	});
}

let lines = fs.readFileSync("input.txt", "utf-8").split("\n");
let matrix = create_matrix(50, 6);

parse_instructions(matrix, lines);

let count = matrix.flatMap(v => {
	return v.filter(c => c == "#").length;
}).reduce((s, v) => s + v, 0);

console.log(`Part 1: ${count}`);
console.log("Part 2:");
print_matrix(matrix);

