// Hajin Jang, Ju Suk Yoon, Jiewan Hong

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "Piece.h"
#include "Board.h"
#include "Exceptions.h"

namespace Chess
{

	class Game {

	public:
		// This default constructor initializes a board with the standard
		// piece positions, and sets the state to white's turn
		Game();

		~Game(); 

		// Returns true if it is white's turn
		/////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
		bool turn_white() const { return is_white_turn; }
    
    /////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
    // Displays the game by printing it to stdout
		void display() const { board.display(); }
    
    /////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
    // Checks if the game is valid
		bool is_valid_game() const { return board.has_valid_kings(); }

		// Attempts to make a move. If successful, the move is made and
		// the turn is switched white <-> black. Otherwise, an exception is thrown
		void make_move(const Position& start, const Position& end);

		// Helper function for make_move to check if diagnol path is clear
		bool check_diagnol_clear(const Position& start, const Position& end) const;

		// Helper function for make_move to check if straight path is clear
		bool check_straight_clear(const Position& start, const Position& end) const;

		// Returns true if the designated player is in check
		bool in_check(const bool& white) const;

		// Returns true if the designated player is in mate
		bool in_mate(const bool& white) const;

		// Returns true if the designated player is in mate
		bool in_stalemate(const bool& white) const;

        // Return the total material point value of the designated player
        int point_value(const bool& white) const;

		// Deep copy maker
		Game* copy_game() const;

		// returns the position of the player's king
		Position find_own_king(const Game* game) const;

		// returns true if the start position is on board
		bool start_pos_onboard(const char c1, const char n1) const;
	
		// returns true if the end position is on board
		bool end_pos_onboard(const char c2, const char n2) const;

		// returns true if there is a piece on the start
		bool piece_start(const Position& start, const Game* game) const;

		// returns true if color of piece and and turn match
		bool color_turn_match(const Position& start, const Game* game) const;

		// returns true if move is legal
		bool legal_move(const Position& start, const Position& end, const Game* game) const;

		// returns true if piece and capturing piece are different colors
		bool capture_own(const Position& start, const Position& end, const Game* game) const; 

		// returns true if capture shape is legal
		bool legal_capture(const Position& start, const Position& end, const Game* game) const; 

		// returns true if path is clear
		bool clear_path(const Position& start, const Position& end, const Game* game) const;

		// clears a board of a game
		void clear_game();

	private:
		// The board
		Board board;

		// Is it white's turn?
		bool is_white_turn;

        // Writes the board out to a stream
        friend std::ostream& operator<< (std::ostream& os, const Game& game);

        // Reads the board in from a stream
        friend std::istream& operator>> (std::istream& is, Game& game);
	};
}
#endif // GAME_H