#pragma once

#include <stdlib.h>
#include <vector>

#include "ratio.hpp"
#include "board.hpp"

namespace ai2 {

namespace detail {

static constexpr int playoffs_per_search = 1000;

double rand01() {
	return (double)rand() / RAND_MAX;
}

struct GameTree {
	bool populated {false};
	bool is_end_state {false};
	Board board;
	Color player;
	Ratio ratio {};
	std::vector<std::pair<int, GameTree>> next;

	GameTree(Board board_, Color player_)
	: board{board_}
	, player{player_}
	{

		auto end_condition = board.is_end_board();
		if (end_condition.first) {
			populated = true;
			is_end_state = true;

			bool won = end_condition.second == player;
			bool lost = end_condition.second == opposite(player);
			ratio = {1, int(won), int(lost)};
		}
	}

	void populate() {
		if (populated) return;
		populated = true;

		for (int i = 0; i < board_width; ++i) {
			if (board.can_move(i)) {
				next.push_back({i, GameTree{board.with_move(i, player), opposite(player)}});
			}
		}

		recompute_ratio();
	}

	void recompute_ratio() {
		ratio = {};
		for (int i = 0; i < int(next.size()); ++i) {
			ratio += next[i].second.ratio.complement();
		}
	}

	int sample() {
		int idx = -1;

		double prob = 0.0;
		for (int i = 0; i < int(next.size()); ++i) {
			GameTree& other = next[i].second;
			other.populate();

			double prob_i = other.lose_probability();
			assert(prob_i >= 0.0);

			if (rand01() * (prob + prob_i) <= prob_i) {
				idx = i;
			}

			prob += prob_i;
		}

		return idx;
	}

	bool has_valid_moves() {
		assert(populated);

		return not next.empty();
	}

	double lose_probability() const {
		assert(populated);

		if (is_end_state) return ratio.complement().as_double();

		Ratio adjusted_ratio = ratio.complement() + Ratio{2, 1, 1};
		return adjusted_ratio.as_double();
	}

	Ratio simulate() {
		if (is_end_state) {
			return ratio;
		}

		int idx = sample();
		assert(idx != -1);

		next[idx].second.populate();
		Ratio result = (next[idx].second.simulate()).complement();

		ratio += result;

		return result;
	}

	int best_move() {
		double best_prob = -1.0;
		int idx = -1;
		for (int i = 0; i < int(next.size()); ++i) {
			GameTree& other = next[i].second;
			double prob_i = other.lose_probability();
			int move = next[i].first;
			std::cerr << "Juega " << move << " ~~~~> " << prob_i << " -- ("<<other.ratio.loses<<":"<<other.ratio.playoffs<<")\n";
			if (prob_i > best_prob) {
				idx = move;
				best_prob = prob_i;
			}
		}
		return idx;
	}

};

} // namespace detail

int next_move(Board const& b, Color player) {
	detail::GameTree gt{b, player};
	gt.populate();
	for (int i = 0; i < detail::playoffs_per_search; ++i) {
		gt.simulate();
	}
	return gt.best_move();
}

} // namespace ai2
