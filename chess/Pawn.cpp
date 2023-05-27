// Hajin Jang, Ju Suk Yoon, Jiewan Hong

#include "Pawn.h"

namespace Chess
{
  bool Pawn::legal_move_shape(const Position& start, const Position& end) const {
    // check if pawn is moving for the first time
    if (is_white() && start.second == '2') { // white & first move
      // move is valid if pawn moves one or two squares 'up'
      if (start.first == end.first && (end.second == start.second + 1 || end.second == start.second + 2)) {
        return true;
      }
      return false;
    }
    else if (!is_white() && start.second == '7') { // black & first move
      // move is valid if pawn moves one or two squares 'down'
      if (start.first == end.first && (end.second == start.second - 1 || end.second == start.second - 2)) {
        return true;
      }
      return false;
    }
    else if (start.first == end.first && (is_white() && start.second != '2')) { // white & not first move
      if (end.second == start.second + 1) {
        return true;
      }
      return false;
    }
    else { // black & not first move
      if (start.first == end.first && (end.second == start.second - 1)) {
        return true;
      }
      return false;
    }
    return false;
  }

  bool Pawn::legal_capture_shape(const Position& start, const Position& end) const {
    // check if capture move is valid
    if (end.first == start.first + 1 || end.first == start.first - 1) {
      if (is_white() && end.second == start.second + 1) {
        return true;
      }
      else if (!is_white() && end.second == start.second - 1) {
        return true;
      }
      return false;
    }
    return false;
  }
}

