#include <iostream>
#include <utility>

#include <cassert>
#include <cstdlib>

static constexpr int board_width = 7;
static constexpr int board_height = 6;
static constexpr int playoffs_per_search = 100000;
static constexpr int line_length = 4;

enum class Color {
	None, Red, Blue
};

struct Board {
	Color data[board_width][board_height] = {};

	std::pair<bool, Color> is_end_board() const {
		for (int i = 0; i < board_width; ++i) {
			for (int j = 0; j < board_height - line_length + 1; ++j) {

				if (data[i][j] == Color::None)
					continue;

				bool good = true;
				for (int k = 0; k < line_length; ++k)
					if (data[i][j] != data[i][j+k])
						good = false;

				if (good)
					return {true, data[i][j]};
			}
		}

		for (int i = 0; i < board_width - line_length + 1; ++i) {
			for (int j = 0; j < board_height; ++j) {

				if (data[i][j] == Color::None)
					continue;

				bool good = true;
				for (int k = 0; k < line_length; ++k)
					if(data[i][j] != data[i+k][j])
						good = false;

				if (good)
					return {true, data[i][j]};
			}
		}

		for(int i = 0; i < board_width; ++i)
			for(int j = 0; j < board_height; ++j)
				if(data[i][j] == Color::None)
					return {false, Color::None};

		// board is full, draw
		return {true, Color::None};
	}

	bool can_move(int move) const {
		return data[move][board_height - 1] == Color::None;
	}

	Board with_move(int move, Color player) const {
		assert(player != Color::None);
		assert(can_move(move));
		Board result = *this;
		int h = 0;
		while (data[move][h] != Color::None)
			h += 1;
		result.data[move][h] = player;
		return result;
	};

	void print() const {
		for (int j = board_height; j--;){
			for (int i = 0; i < board_width; ++i){
				if(data[i][j] == Color::None)
					std::cout << "   ";
				else
					std::cout << (data[i][j] == Color::Red ? "RR" : "BB") << " ";
			}
			std::cout << "\n";
		}
	}
};

struct Ratio {
	int playoffs {0};
	int wins {0};

	Ratio() {};

	Ratio(int playoffs_, int wins_)
		: playoffs{playoffs_}
		, wins{wins_} {}

	Ratio& operator += (Ratio const& o) {
		playoffs += o.playoffs;
		wins += o.wins;
		return *this;
	}

	double as_double() const {
		if (playoffs == 0) return 0.0;
		return (double)wins / playoffs;
	}
};

Color opposite(Color player){
	assert(player != Color::None);
	return player == Color::Red ? Color::Blue : Color::Red;
}

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
		ratios[move] += Ratio {1, winner == player ? 1 : (winner == opposite(player) ? -1 : 0)};
	}

	double best_score = -10000.0;
	int best = -1;

	for (int i = 0; i < board_width; ++i){
		double as_double = ratios[i].as_double();
		if (best_score < as_double) {
			best = i;
			best_score = as_double;
		}
	}

	assert(best != -1);
	return best;
}

int main () {
	Board b;
	Color player = Color::Red;
	
	while (1) {
		if (b.is_end_board().first)
			break;

		std::cout << "Juega el humano\n";
		int player_move;
		std::cin >> player_move;
		while(player_move < 0 || player_move >= board_width || !b.can_move(player_move)) {
			std::cout << "Jugada invalida\n";
			std::cin >> player_move;
		}

		b = b.with_move(player_move, player);
		player = opposite(player);

		b.print();

		if (b.is_end_board().first)
			break;

		std::cout << "Juega la IA\n";
		int ai_move = next_move(b, player);

		b = b.with_move(ai_move, player);
		player = opposite(player);

		b.print();
	}

	b.print();
	std::cout << "El ganador es " << (b.is_end_board().second == Color::Red ? "humano" : "IA") << "\n";
}
