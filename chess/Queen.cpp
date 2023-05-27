// Hajin Jang, Ju Suk Yoon, Jiewan Hong

#include "Queen.h"

namespace Chess
{
  bool Queen::legal_move_shape(const Position& start, const Position& end) const {
    char c1 = start.first;
    char c2 = end.first;
    char n1 = start.second;
    char n2 = end.second;

    // moves diagnolly and up and down without limit
    int row_diff = abs(c1 - c2);
    int col_diff = abs(n1 - n2);

    if (c1 == c2 || n1 == n2) {
      return true;
    } else if (row_diff == col_diff) {
      return true;
    } else {
      return false;
    }
  }
}
