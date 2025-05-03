
#include <ostream>

#include "../include/types.h"

std::ostream &operator<<(std::ostream &os, const Cell &cell) {
  return os << (cell.state == CellState::Empty     ? "[_]"
                : cell.state == CellState::Player1 ? "[1]"
                                                   : "[2]")
            << " <" << cell.position[0] << ", " << cell.position[1] << ">";
}
