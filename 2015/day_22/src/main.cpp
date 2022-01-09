#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>

#include <iostream>
#include <map>

using namespace std;

#define MAX_COST 100000

typedef enum {
    MMissile,
    Drain,
    Shield,
    Poison,
    Recharge,
} SpellID;

static int spell_costs[5] = { 53, 73, 113, 173, 229 };

class Player {
	public:
		int hp = 50;
		int armor = 0;
		int mana = 500;

		Player() = default;
		Player(const Player&) = default;
		~Player() = default;

		Player& operator=(const Player&) = default;
		auto operator<=>(const Player&) const = default;
};

struct Boss {
	public:
		int hp = 71;
		int dmg = 10;

		Boss() = default;
		Boss(const Boss&) = default;
		~Boss() = default;

		Boss& operator=(const Boss&) = default;
		auto operator<=>(const Boss&) const = default;
};

class State {
	public:
		Player player;
		Boss boss;

		int poison_countdown = 0;
		int shield_countdown = 0;
		int recharge_countdown = 0;
		bool player_turn = true;

		bool hard = false;

		State() = default;
		State(const State&) = default;
		~State() = default;

		auto operator<=>(const State&) const = default;

		void magic_missile();
		void drain();
		void shield();
		void poison();
		void recharge();
		void end_turn();
};

void State::magic_missile()
{
    player.mana -= 53;
    boss.hp -= 4;
}

void State::drain()
{
    player.mana -= 73;
    player.hp += 2;
    boss.hp -= 2;
}

void State::shield()
{
	if (shield_countdown == 0) {
		player.mana -= 113;
		shield_countdown = 6;
		player.armor = 7;
	}
}

void State::poison()
{
	if (poison_countdown == 0) {
		player.mana -= 173;
		poison_countdown = 6;
	}
}

void State::recharge()
{
	if (recharge_countdown == 0) {
		player.mana -= 229;
		recharge_countdown = 5;
	}
}

void State::end_turn()
{
	player_turn = !player_turn;
}

State cast(State *current, SpellID spell)
{
    State state(*current);

    switch (spell) {
        case MMissile:
            state.magic_missile();
            break;
        case Drain:
            state.drain();
            break;
        case Shield:
            state.shield();
            break;
        case Poison:
            state.poison();
            break;
        case Recharge:
            state.recharge();
            break;
    };
	state.end_turn();
    return state;
}

int step(State *state, std::map<State, int> *cache)
{
    if (state->shield_countdown > 0) {
		state->shield_countdown--;
	}

	if (state->shield_countdown > 0) {
		state->player.armor = 7;
	} else {
		state->player.armor = 0;
	}

	if (state->hard && state->player_turn) {
		state->player.hp -= 1;
	}

    if (state->poison_countdown > 0) {
        state->poison_countdown--;
        state->boss.hp -= 3;
    }
    if (state->recharge_countdown > 0) {
        state->recharge_countdown--;
        state->player.mana += 101;
    }

    if (state->player.hp <= 0 || state->player.mana < 53) {
        return MAX_COST;
    } else if (state->boss.hp <= 0) {
        return 0;
    }

    int min_mana_cost = MAX_COST;
    if (state->player_turn) {
		if (cache->find(*state) != cache->end()) {
			min_mana_cost = (*cache)[*state];
		} else {
			for (size_t i = 0; i <= Recharge; ++i) {
				if (spell_costs[i] > state->player.mana) {
					// Spell to expensive. Skip
					continue;
				}

				State next = cast(state, static_cast<SpellID>(i));
				int mana_cost = state->player.mana - next.player.mana;

				if (mana_cost == 0) {
					// Invalid spell. Skip
					continue;
				}

				mana_cost += step(&next, cache);
				if (min_mana_cost > mana_cost)
					min_mana_cost = mana_cost;
			}
			(*cache)[State(*state)] = min_mana_cost;
		}
    } else {
		State next(*state);
        next.player.hp -= (next.boss.dmg - next.player.armor);
		next.end_turn();
        min_mana_cost = step(&next, cache);
		(*cache)[State(*state)] = min_mana_cost;
    }

    return min_mana_cost;
}

int main()
{
	auto cache = new std::map<State, int>();
    State state;
    printf("Part 1: %d\n", step(&state, cache));

	// Reset the state and enable hardmode
	cache->clear();
	state = State();
	state.hard = true;
    printf("Part 2: %d\n", step(&state, cache));
	delete cache;
}
