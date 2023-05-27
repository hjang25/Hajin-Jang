// Hajin Jang, Ju Suk Yoon, Jiewan Hong

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Mystery.h"


namespace Chess
{
  class Board {

		// Throughout, we will be accessing board positions using Position defined in Piece.h.
		// The assumption is that the first value is the column with values in
		// {'A','B','C','D','E','F','G','H'} (all caps)
		// and the second is the row, with values in {'1','2','3','4','5','6','7','8'}

	public:
		// Default constructor
		Board();

		~Board();

		// Returns a const pointer to the piece at a prescribed location if it exists,
		// or nullptr if there is nothing there.
		const Piece* operator() (const Position& position) const;

		// Attempts to add a new piece with the specified designator, at the given position.
		// Throw exception for the following cases:
		// -- the designator is invalid, throw exception with error message "invalid designator"
		// -- the specified position is not on the board, throw exception with error message "invalid position"
		// -- if the specified position is occupied, throw exception with error message "position is occupied"
		void add_piece(const Position& position, const char& piece_designator);

		// Displays the board by printing it to stdout
		void display() const;

		// Returns true if the board has the right number of kings on it
		bool has_valid_kings() const;

		// Remove piece from board
		void remove_piece(const Position &position);

		// Deallocate all board pieces
		void clean();

		std::map<Position, Piece*> get_occ() {return occ;}

		class iterator {
			private:
			Position ptr; 

			public:
			iterator(Position initial) : ptr(initial) {};

			const iterator& operator++() {
				char x = ptr.first;
				char y = ptr.second;

				if (x == 'H') {
					if (y == '1') {
						ptr = Position(-1, -1);
					} else {
						ptr = Position('A', y - 1);
					} 
				} else {
					ptr = Position(x + 1, y);
				}

				return *this;
			} 

			bool operator!=(const iterator& o) const {
				return !(*this == o);
			}

			bool operator==(const iterator& o) const {
				return (ptr.first == o.ptr.first && ptr.second == o.ptr.second);
			}

    		const Position& operator*() const { 
				return ptr;
			}
		};
			iterator cbegin() const {
				return iterator(Position('A', '8')); 
			}

			iterator cend() const {
				return iterator(Position(-1, -1));
			}

	private:
		// The sparse map storing the pieces, keyed off locations
		std::map<Position, Piece*> occ;

        // Write the board state to an output stream
        friend std::ostream& operator<< (std::ostream& os, const Board& board);
	};
}
#endif // BOARD_H
