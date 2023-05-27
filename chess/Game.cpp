// Hajin Jang, Ju Suk Yoon, Jiewan Hong

#include <cassert>
#include "Game.h"
#include <map>

using std::map;

namespace Chess
{
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	Game::Game() : is_white_turn(true) {
		// Add the pawns
		for (int i = 0; i < 8; i++) {
			board.add_piece(Position('A' + i, '1' + 1), 'P');
			board.add_piece(Position('A' + i, '1' + 6), 'p');
		}

		// Add the rooks
		board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
		board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

		// Add the knights
		board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
		board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

		// Add the bishops
		board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
		board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

		// Add the kings and queens
		board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
		board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
		board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
		board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
	}

	Game::~Game() { 
		this->clear_game();
	}

	void Game::make_move(const Position& start, const Position& end) {
		// To check if start position exists on the board 
   		if (!start_pos_onboard(start.first, start.second)) {
      		throw Exception("start position is not on board");
    	} 
 		
		// To check if end position exists on the board 
		if (!end_pos_onboard(end.first, end.second)) {
      		throw Exception("end position is not on board");
    	} 
		
		// no piece at start position
		if (!piece_start(start, this)) {
			throw Exception("no piece at start position");
		}

		// piece color and turn do not match
		if (!color_turn_match(start, this)) {
			throw Exception("piece color and turn do not match");
		}

		if (board(end) == nullptr) {
			// illegal move shape
			if (!legal_move(start, end, this)) {
				throw Exception("illegal move shape");
			}
		} else {
			// cannot capture own piece
			if (!capture_own(start, end, this)) {
				throw Exception("cannot capture own piece");
			}
			// illegal capture shape
			if (!legal_capture(start, end, this)) { 
				throw Exception("illegal capture shape");
			} 
		}

		// path is not clear
		if (!clear_path(start, end, this)) {
			throw Exception("path is not clear");
		}

		// move exposes check
		Game* copy = copy_game(); // create game replica
		if (copy->board(end) != NULL) { 
			copy->board.remove_piece(end);
		}
		copy->board.add_piece(end, board(start)->to_ascii());
		copy->board.remove_piece(start);

		if (copy->in_check(copy->turn_white())) {
			delete(copy); // free memory allocated to copy before throwing exception
			throw Exception("move exposes check");
		}
		
		// delete copy when confirmed that move does not expose check
		delete(copy);

		// if no exceptions thrown and opponent's piece exists at end position, capture
		if (this->board(end) != NULL) { // already checked that piece at the end is not one's own
			this->board.remove_piece(end);
		}

		// update map with changed pointers
		this->board.add_piece(end, board(start)->to_ascii());
		board.remove_piece(start);

		// if pawn reaches the opposite end, promote to Queen
		if (this->turn_white()) {
			if (end.second == '8' && this->board(end)->to_ascii() == 'P') {
				board.remove_piece(end); 
				this->board.add_piece(end, 'Q');
			}
		} else { // promotion for black pawn case
			if (end.second == '1' && this->board(end)->to_ascii() == 'p') {
				board.remove_piece(end); 
				this->board.add_piece(end, 'q');
			}
		}

		// switch turns after capturing/moving
		if (this->turn_white()) {
			this->is_white_turn = false;
		} else { 
			this->is_white_turn = true;
		}
	}
	
	bool Game::in_check(const bool& white) const {
		// find white king if white's turn and vice versa using helper function 'find_own_king'
		Position king_pos = find_own_king(this);

		// iterate over the entire board (from tile A8 to H1)
		for (Board::iterator it = this->board.cbegin(); it != this->board.cend(); ++it) {
			if (this->board(*it) != nullptr) {
				if (white && !(this->board(*it)->is_white())) {
					if ((clear_path(*it, king_pos, this)) && (legal_capture(*it, king_pos, this))) {
						return true;
					}
				}
				if (!white && this->board(*it)->is_white()) {
					if ((clear_path(*it, king_pos, this)) && (legal_capture(*it, king_pos, this))) {
						return true;
					}
				}
			}
		} 
		return false;
	}

	bool Game::in_mate(const bool& white) const {
		// check whether moving any of the player's pieces can escape check
		if (!this->in_check(this->turn_white())) {
			return false;
		}

		for (Board::iterator it = this->board.cbegin(); it != this->board.cend(); ++it) {
			if (this->board(*it) != nullptr && white == this->board(*it)->is_white()) {
				for (Board::iterator fit = this->board.cbegin(); fit != this->board.cend(); ++fit) {
					Game* copy = copy_game();
					try {
						copy->make_move(*it, *fit);
						delete(copy); 
						return false; 
					}
					catch (const std::exception &c) { 
						delete(copy);
					}
				}
			}
		} 
		return true;
	}

	bool Game::in_stalemate(const bool& white) const {
		for (Board::iterator it = this->board.cbegin(); it != this->board.cend(); ++it) {
			if (this->board(*it) != nullptr && white == this->board(*it)->is_white()) {
				for (Board::iterator fit = this->board.cbegin(); fit != this->board.cend(); ++fit) {
					Game* copy = copy_game();
					try {
						copy->make_move(*it, *fit);
						delete(copy);
						return false; 
					}
					catch (const std::exception &c) { 
						delete(copy); 
					}
				}
			}
		}
		return true;
	}

    // Return the total material point value of the designated player
    int Game::point_value(const bool& white) const {
		int sum = 0;

		for (Board::iterator it = board.cbegin(); it != board.cend(); ++it) {	
		
			if (board(*it) != nullptr && white == board(*it)->is_white()) {
				//char c = board(*it)->to_ascii();
				sum += board(*it)->point_value();
			}

		}
        return sum;
    }

	// Read the board in from a stream 
    std::istream& operator>> (std::istream& is, Game& game) {
		
		// throw exception if invalid is given
		if (!is) {
			throw Exception("Cannot load the game!");
		}

		// remove all preexisting pieces from board 
		game.board.clean(); 

		// add specified pieces to board
		try {
			for (int row = '8'; row >= '1'; row--) {
				for (int col = 'A'; col <= ('H' + 1); col++) {
					char piece_designator = is.get();
					if (piece_designator != '-' && piece_designator != '\n') {
						game.board.add_piece(Position(col, row), piece_designator);
					}
				}
			}
		}
		catch (const std::exception &exc) {
			throw Exception("Cannot load the game!");
		}

		// designate turn
		char turn_designator = is.get();

		if (turn_designator != 'w' && turn_designator != 'b') {
        	throw Exception("Cannot load the game!");
        }

		// store whose turn it is
		if (turn_designator == 'w') {
			game.is_white_turn = true;
		} else {
			game.is_white_turn = false;
		}
		
		return is;
	}

    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
	std::ostream& operator<< (std::ostream& os, const Game& game) {
		// Write the board out and then either the character 'w' or the character 'b',
		// depending on whose turn it is
		return os << game.board << (game.turn_white() ? 'w' : 'b');
	}

	bool Game::start_pos_onboard(const char c1, const char n1) const { 

		if (c1 < 'A' || c1 > 'H' || n1 < '1' || n1 > '8') {
      		return false;
    	} 
		return true;
	}

	bool Game::end_pos_onboard(const char c2, const char n2) const { 

		if (c2 < 'A' || c2 > 'H' || n2 < '1' || n2 > '8') {
      		return false;
    	} 
		return true;
	}

	bool Game::piece_start(const Position& start, const Game* game) const { 
		if (game->board(start) == nullptr) {
			return false;
		}
		return true;
	}

	bool Game::color_turn_match(const Position& start, const Game* game) const { 
		if (game->board(start)->is_white() != game->turn_white()) {
			return false;
		}
		return true;
	}

	bool Game::legal_move(const Position& start, const Position& end, const Game* game) const { 
		if (!game->board(start)->legal_move_shape(start, end)) {
			return false;
		}
		return true;
	}

	bool Game::capture_own(const Position& start, const Position& end, const Game* game) const { 
		if (game->board(start) != nullptr && game->board(end) != nullptr) {
			if (game->board(start)->is_white() == game->board(end)->is_white()) {
				return false;
			}
			return true;
		}
		return true;
	}	
		
	bool Game::legal_capture(const Position& start, const Position& end, const Game* game) const {
		if (!game->board(start)->legal_capture_shape(start, end)) {
			return false;
		} 
		return true;
	}

	bool Game::clear_path(const Position& start, const Position& end, const Game* game) const {	
		int row_diff = abs(start.second - end.second);
		int col_diff = abs(start.first - end.first); 
		
		if (game->board(start)->to_ascii() == 'p' || game->board(start)->to_ascii() == 'P') {
			if (start.first == end.first) {
				if (game->board(start)->is_white()) {
					if (row_diff == 2) {
						if (game->board(Position(start.first, start.second + 1)) != nullptr || game->board(Position(start.first, start.second + 2)) != nullptr) {
							return false;
						}
					} else if (row_diff == 1) {
						if (game->board(Position(start.first, start.second + 1)) != nullptr) {
							return false;
						}
					}
				} else {
					if (row_diff == 2) {
						if (game->board(Position(start.first, start.second - 1)) != nullptr || game->board(Position(start.first, start.second - 2)) != nullptr) {
							return false;
						}
					} else if (row_diff == 1) {
						if (game->board(Position(start.first, start.second - 1)) != nullptr) {
							return false;
						}
					} else {
						if (game->board(end) == nullptr) {
							return false;
						}
					}
				}
			}
		} else { 
			if (start.first == end.first || start.second == end.second) {
				if (!check_straight_clear(start, end)) {
					return false;
				}
			} else if (row_diff == col_diff) {
				if (!check_diagnol_clear(start, end)) {
					return false;
				}
			}
		}
		return true;
	}

	bool Game::check_straight_clear(const Position& start, const Position& end) const {
		int row_diff = abs(start.second - end.second);
		int col_diff = abs(start.first - end.first); 

		if (start.first == end.first && start.second < end.second) {
			for (int i = 1; i < row_diff; i++) {
				if (board(Position(start.first, start.second + i)) != nullptr) {
					return false;
				}
			}
		} else if (start.first == end.first && start.second > end.second) {
			for (int i = 1; i < row_diff; i++) {
				if (board(Position(start.first, start.second - i)) != nullptr) {
					return false;
				}
			}
		} else if (start.second == end.second && start.first < end.first) {
			for (int i = 1; i < col_diff; i++) {
				if (board(Position(start.first + i, start.second)) != nullptr) {
					return false;
				}
			}
		} else if (start.second == end.second && start.first > end.first) {
			for (int i = 1; i < col_diff; i++) {
				if (board(Position(start.first - i, start.second)) != nullptr) {
					return false;
				}
			}
		} 
		return true;	
	}

	bool Game::check_diagnol_clear(const Position& start, const Position& end) const {
		int diff = abs(start.second - end.second);

		if (start.first > end.first && start.second < end.second) {
			for (int i = 1; i < diff; i++) {
				if (board(Position(start.first - i, start.second + i)) != nullptr) {
					return false;
				}
			}
		} else if (start.first < end.first && start.second < end.second) {
			for (int i = 1; i < diff; i++) {
				if (board(Position(start.first + i, start.second + i)) != nullptr) {
					return false;
				}
			}
		} else if (start.first < end.first && start.second > end.second) {
			for (int i = 1; i < diff; i++) {
				if (board(Position(start.first + i, start.second - i)) != nullptr) {
					return false;
				}
			}
		} else if (start.first > end.first && start.second > end.second) {
			for (int i = 1; i < diff; i++) {
				if (board(Position(start.first - i, start.second - i)) != nullptr) {
					return false;
				}
			}
		}
		return true;
	}

	Game* Game::copy_game() const {
		Game* new_game = new Game();
		new_game->board.clean();
		new_game->is_white_turn = this->turn_white();
		
		for (Board::iterator it = board.cbegin(); it != board.cend(); ++it) {
			if (board(*it) != nullptr) {
				char piece_type = board(*it)->to_ascii();
				(*new_game).board.add_piece(*it, piece_type);
			}
		}
		return new_game;
	}
	
	void Game::clear_game() {
		board.clean();
	}

	Position Game::find_own_king(const Game* game) const {
		// iterate over the board to find king
		for (Board::iterator it = (*game).board.cbegin(); it != (*game).board.cend(); ++it) {
			if ((*game).board(*it) != nullptr) {
				// find white king
				if (game->turn_white() && game->board(*it)->to_ascii() == 'K') {
					return *it;
				}
				// find black king
				else if (!(game->turn_white()) && game->board(*it)->to_ascii() == 'k') {
					return *it;
				}
			}
		}
		// return null char (an invalid Position) if there was an error in finding king
		return std::make_pair('\0', '\0'); // can assume a king always exists as long as not end-of-game
	}
}