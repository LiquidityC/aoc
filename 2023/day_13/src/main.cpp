#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;

using Section = std::vector<string>;
using Sections = std::vector<Section>;

static Sections read_lines()
{
    std::ifstream in("input.txt");

    Sections sections;
    Section section;
    string line;
    while (std::getline(in, line)) {
        if (line.empty()) {
            sections.push_back(section);
            section.clear();
        } else {
            section.push_back(line);
        }
    }
    sections.push_back(section);

    in.close();
    return sections;
}

static bool is_reflection(const Section& section, size_t index)
{
    auto fit = section.begin() + index + 1;
    auto rit = section.rbegin() + (section.size() - index - 1);

    while (fit != section.end() && rit != section.rend()) {
        if (*fit != *rit) {
            return false;
        }
        fit++;
        rit++;
    }
    return true;
}

static int find_reflection(const Section& section)
{
    for (size_t i = 0; i < section.size() - 1; ++i) {
        auto r1 = section[i];
        auto r2 = section[i + 1];
        if (r1 == r2 && is_reflection(section, i)) {
            return i;
        }
    }

    return -1;
}

static Section rotate(const Section& s)
{
    Section rot;
    for (size_t i = 0; i < s[0].length(); ++i) {
        string row;
        for (size_t j = 0; j < s.size(); ++j) {
            row += s[j][i];
        }
        rot.push_back(row);
    }
    return rot;
}

static void part1(const Sections& sections)
{
    size_t row_count = 0;
    size_t col_count = 0;
    for (auto &sec : sections) {
        auto r = find_reflection(sec);
        if (r >= 0) {
            row_count += r + 1;
            continue;
        }
        auto c = find_reflection(rotate(sec));
        if (c >= 0) {
            col_count += c + 1;
            continue;
        }

        throw std::exception();
    }

    cout << "Part 1: " << col_count + 100 * row_count << endl;
}

size_t count_differences(const string& a, const string& b)
{
    size_t count = 0;
    for (size_t i = 0; i < a.length(); ++i) {
        if (a[i] != b[i]) {
            count++;
        }
        if (count > 1) {
            break;
        }
    }
    return count;
}

static int find_fixed_reflection(Section section)
{
    for (size_t i = 0; i < section.size(); ++i) {
        auto& r = section[i];
        for (size_t j = i + 1; j < section.size(); ++j) {
            auto& cmp = section[j];

            if (count_differences(r, cmp) == 1) {
                auto bck = r;
                r = cmp;

                if (is_reflection(section, (i+j)/2)) {
                    return (i+j)/2;
                }
                r = bck;
            }
        }
    }
    return -1;
}

static void part2(const Sections& sections)
{
    size_t row_count = 0;
    size_t col_count = 0;
    for (auto &sec : sections) {
        auto r = find_fixed_reflection(sec);
        if (r >= 0) {
            row_count += r + 1;
            continue;
        }
        auto c = find_fixed_reflection(rotate(sec));
        if (c >= 0) {
            col_count += c + 1;
            continue;
        }
        throw std::exception();
    }

    cout << "Part 2: " << col_count + 100 * row_count << endl;
}

int main()
{

    auto sections = read_lines();

    part1(sections);
    part2(sections);

    return 0;
}
