// Hajin Jang, Ju Suk Yoon, Jiewan Hong

#include "Knight.h"

namespace Chess
{
  bool Knight::legal_move_shape(const Position& start, const Position& end) const {
    // moves in L-shape
    int row_diff = abs(start.first - end.first);
    int col_diff = abs(start.second - end.second);

    if ((row_diff == 2 && col_diff == 1) || (col_diff == 2 && row_diff == 1)) {
      return true;
    } else {
      return false;
    }
  }
}
