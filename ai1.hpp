#pragma once

#include "board.hpp"
#include "ratio.hpp"

namespace ai1 {

namespace detail {

static constexpr int playoffs_per_search = 100;


Color simulate(Board b, Color player) {
	while (1) {
		if (auto result = b.is_end_board(); result.first)
			return result.second;

		int move = rand() % board_width;
		while (!b.can_move(move))
			move = rand() % board_width;

		b = b.with_move(move, player);
		player = opposite(player);
	}
}

int next_move(Board const& b, Color player) {
	assert(player != Color::None);
	assert(!b.is_end_board().first);

	Ratio ratios[board_width];

	for (int i = 0; i < playoffs_per_search; ++i) {

		int move = rand() % board_width;
		while (!b.can_move(move))
			move = rand() % board_width;

		Board b_i = b.with_move(move, player);
		Color winner = simulate(b_i, opposite(player));
		ratios[move] += Ratio {1, winner == player ? 1 : 0, winner == opposite(player) ? 1 : 0};
	}

	double best_score = -10000.0;
	int best = -1;

	for (int i = 0; i < board_width; ++i){
		double as_double = ratios[i].as_double();
		std::cerr << "play " << i << " ~~~~> " << as_double << '\n';
		if (best_score < as_double) {
			best = i;
			best_score = as_double;
		}
	}

	assert(best != -1);
	return best;
}

} // namespace detail

int next_move(Board const& b, Color player) {
	return detail::next_move(b, player);
}

} // namespace ai1
