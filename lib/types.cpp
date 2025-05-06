
#include <ostream>

#include "../include/types.h"

std::ostream &operator<<(std::ostream &os, const Cell &cell) {
  return os << (cell.state == CellState::Empty     ? "[_]"
                : cell.state == CellState::Player1 ? "(O)"
                                                   : "[X]")
            << " <" << (cell.position[1] + 1) << ", " << (cell.position[0] + 1)
            << ">";
}
