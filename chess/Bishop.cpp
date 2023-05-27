// Hajin Jang, Ju Suk Yoon, Jiewan Hong

#include "Bishop.h"

namespace Chess
{
  bool Bishop::legal_move_shape(const Position& start, const Position& end) const {
    char c1 = start.first;
    char c2 = end.first;
    char n1 = start.second;
    char n2 = end.second;
    
    // only move diagnolly
    int row_diff = abs(c1 - c2);
    int col_diff = abs(n1 - n2);

    if (row_diff == col_diff) {
      return true;
    } else {
      return false;
    }
  }
}
