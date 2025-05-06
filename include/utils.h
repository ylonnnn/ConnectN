#pragma once

#include <random>
#include <vector>

double random();

template <typename T> T &randomElement(std::vector<T> &vec) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, vec.size() - 1);

  return vec[dist(gen)];
}