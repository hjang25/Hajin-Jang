// Hajin Jang, Ju Suk Yoon, Jiewan Hong

#include "King.h"

namespace Chess
{
  bool King::legal_move_shape(const Position& start, const Position& end) const {
    // move all direction by a unit of 1
    int row_diff = abs(start.first - end.first);
    int col_diff = abs(start.second - end.second);

    if ((row_diff == 1 || col_diff == 1) && row_diff + col_diff <= 2) {
      return true;
    } else {
      return false;
    }
  }
}
