// Hajin Jang, Ju Suk Yoon, Jiewan Hong

#include <iostream>
#include <utility>
#include <map>
#include <cctype>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
// Hajin Jang, Ju Suk Yoong, Jiewan Hong
// hjang15, jyoon53, jhong68

#include "CreatePiece.h"
#include "Exceptions.h"

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  Board::Board(){}

	Board::~Board() { 
		this->clean();
	}

  const Piece* Board::operator()(const Position& position) const {
    // If piece at "position" exists, return pointer 
    // If not, return nullptr
    std::map<Position, Piece*>::const_iterator it = occ.find(position);

    if (it == occ.end()) {
      return nullptr;
    }
    return it->second;
  }
  
  void Board::add_piece(const Position& position, const char& piece_designator) {
    // create piece
    Piece* piece = create_piece(piece_designator);
    
    // throw invalid piece designator
    if (piece == nullptr) {
      throw Exception("invalid designator");
    }

    // // throw invalid position
    if (position.first < 'A' || position.first > 'H' || position.second < '1' || position.second > '8') {
      delete(piece);
      throw Exception("invalid position");
    }
    
    // check if specified position is already occupied
    if ((*this)(position) != nullptr) {
      delete(piece);
      throw Exception("position is occupied");
    }

    // if no errors detected, create new piece using helper function
    occ[position] = piece;
  }

  void Board::display() const {
    int count = 0;
    int count_rows = 8;
    std::cout << " " << count_rows << " ";
    for (iterator it = cbegin(); it != cend(); ++it) {
      const Piece* piece = (*this)(*it);
      if (piece != nullptr) {
        // set font color to black/white
        if (piece->is_white()) {
          Terminal::color_fg(true, Terminal::MAGENTA);
        }
        if (!(piece->is_white())) {
          Terminal::color_fg(true, Terminal::BLUE);
        }
        // alternate background color between green and orange
        if (((*it).first) % 2 != 0 && ((*it).second) % 2 == 0) {
          Terminal::color_bg(Terminal::GREEN);
        }
        if (((*it).first) % 2 == 0 && ((*it).second) % 2 != 0) {
          Terminal::color_bg(Terminal::GREEN);
        }
        if (((*it).first) % 2 == 0 && ((*it).second) % 2 == 0) {
          Terminal::color_bg(Terminal::YELLOW);
        }
        if (((*it).first) % 2 != 0 && ((*it).second) % 2 != 0) {
          Terminal::color_bg(Terminal::YELLOW);
        }
        // display character
        std::cout << " " << piece->to_ascii() << " ";
        count++; 
      } else {
                // alternate background color between green and orange
        if (((*it).first) % 2 != 0 && ((*it).second) % 2 == 0) {
          Terminal::color_bg(Terminal::GREEN);
        }
        if (((*it).first) % 2 == 0 && ((*it).second) % 2 != 0) {
          Terminal::color_bg(Terminal::GREEN);
        }
        if (((*it).first) % 2 == 0 && ((*it).second) % 2 == 0) {
          Terminal::color_bg(Terminal::YELLOW);
        }
        if (((*it).first) % 2 != 0 && ((*it).second) % 2 != 0) {
          Terminal::color_bg(Terminal::YELLOW);
        }
        std::cout << "   ";
        count++;
      }

      if (count == 8) {
        Terminal::set_default();
        std::cout << std::endl;
        if (count_rows > 1) {
          Terminal::set_default();
          Terminal::color_fg(true, Terminal::WHITE);
          std::cout << " " << --count_rows << " ";
        } 
        count = 0;
      } 
    }
  
    // set font color back to white
    Terminal::set_default();
    Terminal::color_fg(true, Terminal::WHITE);
    std::cout << "   ";
    // print last row (A to H)
    for (int i = 0; i < 8; i++) {
      std::cout << " " << (char)('A' + i) << " ";
    }  
    std::cout << std::endl;
  }

  void Board::remove_piece(const Position &position) {
    // find Piece pointer at 'position'
    std::map<Position, Piece*>::const_iterator it = occ.find(position);

    // delete if a piece exists at 'position'
    if (occ.cend() != it) {
      delete it->second;
      occ.erase(position);
    }
  }

  void Board::clean() {
    for (iterator it = this->cbegin(); it != this->cend(); ++it) {	
      if ((*this)(*it) != nullptr) {
        delete (*this)(*it);
      }
    }
    occ.clear();
  }

  bool Board::has_valid_kings() const {
    int white_king_count = 0;
    int black_king_count = 0;
    for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
	    it != occ.end(); it++) {
      if (it->second) {
	      switch (it->second->to_ascii()) {
	        case 'K':
	          white_king_count++;
	          break;
	        case 'k':
	          black_king_count++;
	          break;
	      }
      }
    }
    return (white_king_count == 1) && (black_king_count == 1);
  }

  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
	const Piece* piece = board(Position(c, r));
	if (piece) {
	  os << piece->to_ascii();
	} else {
	  os << '-';
	}
      }
      os << std::endl;
    }
    return os;
  }
}
