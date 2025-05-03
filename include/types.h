#pragma once

#include <array>
#include <ostream>
#include <vector>

enum CellState { Empty, Player1, Player2 };

struct Cell {
  CellState state;
  int position[2];
};

using GridSize = std::array<int, 2>;
using Grid = std::vector<std::vector<Cell>>;

using Orientation = std::vector<Cell>;

std::ostream &operator<<(std::ostream &os, const Cell &cell);

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
  os << "{ ";

  int vSize = vec.size();

  for (int i = 0; i < vSize; i++) {
    T el = vec[i];

    os << el << (i < (vSize - 1) ? ", " : "");
  }

  return os << " }";
}
