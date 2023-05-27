// Hajin Jang, Ju Suk Yoon, Jiewan Hong

#include "Rook.h"

namespace Chess
{
  bool Rook::legal_move_shape(const Position& start, const Position& end) const {
    char c1 = start.first;
    char c2 = end.first;
    char n1 = start.second;
    char n2 = end.second;

  // check if moving only in horizontal or vertical direction
  if (c1 == c2 || n1 == n2) {
    return true;
  }
    return false;
  }
}
