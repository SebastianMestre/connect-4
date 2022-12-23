#include <iostream>
#include <utility>

#include <cassert>
#include <cstdlib>

#include "board.hpp"
#include "ai1.hpp"
#include "ai2.hpp"
#include "player.hpp"

char const* get_name(Color c) {
	if (c == Color::Red) {
		return "el humano";
	} else if (c == Color::Blue) {
		return "la IA";
	} else {
		return "NADIE: hay empate!";
	}
}

int get_next_move(Board& b, Color& player) {
	if (player == Color::Red) {
		return player::next_move(b, player);
	} else {
		return ai2::next_move(b, player);
	}
}

int main () {
	Board b;
	Color player = Color::Red;
	
	b.print();
	while (not b.is_end_board().first) {
		std::cout << "Juega " << get_name(player) << "\n";
		int move = get_next_move(b, player);
		if (not b.can_move(move)) {
			std::cout << "Jugada invalida de " << get_name(player) << "\n";
			break;
		}
		b = b.with_move(move, player);
		player = opposite(player);
		b.print();
	}

	Color winner = b.is_end_board().first
		? b.is_end_board().second
		: opposite(player);

	auto winner_name = get_name(winner);
	std::cout << "El ganador es " << winner_name << "\n";
}
