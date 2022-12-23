#pragma once

static constexpr int board_width = 7;
static constexpr int board_height = 6;
static constexpr int line_length = 4;

enum class Color {
	None, Red, Blue
};

Color opposite(Color player){
	assert(player != Color::None);
	return player == Color::Red ? Color::Blue : Color::Red;
}

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
			std::cout << "|";
			for (int i = 0; i < board_width; ++i){
				if(data[i][j] == Color::None)
					std::cout << "   ";
				else
					std::cout << (data[i][j] == Color::Red ? "--" : "##") << " ";
			}
			std::cout << "|\n";
		}
	}
};
