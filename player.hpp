#pragma once

namespace player {

int next_move(Board const& b, Color player) {
	int player_move;
	std::cin >> player_move;
	while (player_move < 0 || player_move >= board_width || !b.can_move(player_move)) {
		std::cout << "Jugada invalida\n";
		std::cin >> player_move;
	}
	return player_move;
}

}
