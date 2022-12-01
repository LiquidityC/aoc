const fs = require("fs");

function read_data(lines) {
	let calories = Array();
	let elf = 0;

	lines.forEach(l => {
		if (l == "") {
			calories.push(elf);
			elf = 0;
		} else {
			elf += parseInt(l);
		}
	});
	calories.push(elf)
	return calories;
}

let data = fs.readFileSync("input.txt", "utf-8");
let calories = read_data(data.split("\n")).sort();
let max = calories[calories.length-1];
let top3 = calories.slice(-3).reduce((sum, i) => sum + i, 0);

console.log(`Part 1: ${max}`);
console.log(`Part 2: ${top3}`);
