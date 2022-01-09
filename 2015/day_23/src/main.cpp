#include <cstdint>
#include <optional>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

class CPU;

class Instruction
{
	private:
		string op;
		optional<char> reg;
		optional<int32_t> offset;

	public:
		Instruction() = default;
		~Instruction() = default;
		Instruction(const Instruction&) = default;
		Instruction& operator=(const Instruction&) = default;

		friend istream& operator>>(istream&, Instruction&);
		friend ostream& operator<<(ostream&, const Instruction&);

		friend class CPU;
};

istream& operator>>(istream& cin, Instruction& i)
{
	i.reg.reset();
	i.offset.reset();

	cin >> i.op;
	if (i.op == "jio" || i.op == "jie")
	{
		char reg;
		int32_t of;
		string skip;
		cin >> reg;
		getline(cin, skip, ' ');
		cin >> of;
		i.reg = reg;
		i.offset = of;
	} else if (i.op == "hlf" || i.op == "inc" || i.op == "tpl") {
		char reg;
		cin >> reg;
		i.reg = reg;
	} else if (i.op == "jmp") {
		int32_t of;
		cin >> of;
		i.offset = of;
	}
	return cin;
}

ostream& operator<<(ostream& out, const Instruction& i)
{
	out << i.op;
		out << "(";
		out << i.reg.value_or('x');
		out << ", " << i.offset.value_or(0) << ")";
	return out;
}

class CPU
{
	private:
		map<char, uint32_t> _registers = {
			{ 'a', 0 },
			{ 'b', 0 }
		};

	public:
		CPU() = default;
		~CPU() = default;
		CPU(const CPU&) = default;
		CPU& operator=(const CPU&) = default;

		const map<char, uint32_t>& registers() const;
		map<char, uint32_t>& registers();

		void exec(const vector<Instruction>&);
};

const map<char, uint32_t>& CPU::registers() const
{
	return _registers;
}

map<char, uint32_t>& CPU::registers()
{
	return _registers;
}

void CPU::exec(const vector<Instruction>& instr)
{
	size_t index = 0;
	while (index < instr.size()) {
		const Instruction& i = instr[index];
		if (i.op == "inc") {
			_registers[*i.reg] += 1;
			index += 1;
		} else if (i.op == "tpl") {
			_registers[*i.reg] *= 3;
			index += 1;
		} else if (i.op == "hlf") {
			_registers[*i.reg] /= 2;
			index += 1;
		} else if (i.op == "jie") {
			if (_registers[*i.reg] % 2 == 0)
				index += *i.offset;
			else
				index += 1;
		} else if (i.op == "jio") {
			if (_registers[*i.reg] == 1)
				index += *i.offset;
			else
				index += 1;
		} else { // jmp
			index += i.offset.value_or(1);
		}
	}
}

int main()
{
	vector<Instruction> instructions;
	ifstream input("input.txt");
	Instruction instr;
	while (input >> instr) {
		instructions.push_back(instr);
	}
	input.close();

	CPU cpu;
	cpu.exec(instructions);
	cout << "Part 1: " << cpu.registers()['b'] << endl;

	cpu.registers()['a'] = 1;
	cpu.registers()['b'] = 0;
	cpu.exec(instructions);
	cout << "Part 2: " << cpu.registers()['b'] << endl;
	return 0;
}
