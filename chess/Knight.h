// Hajin Jang, Ju Suk Yoon, Jiewan Hong

#ifndef KNIGHT_H
#define KNIGHT_H

// Hajin Jang, Ju Suk Yoong, Jiewan Hong
// hjang15, jyoon53, jhong68

#include "Piece.h"

namespace Chess
{
	class Knight : public Piece {

	public:
	int point_value() const override {return 3;} 
	
	bool legal_move_shape(const Position& start, const Position& end) const override;

		/////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
		char to_ascii() const override { return is_white() ? 'N' : 'n';	}
    
    /////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
		std::string to_unicode() const override { return is_white() ? "\u2658" : "\u265E"; }

	private:
		/////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
		Knight(bool is_white) : Piece(is_white) {}

		friend Piece* create_piece(const char& piece_designator);
	};
}

#endif // KNIGHT_H
