#include <random>

#include "../include/utils.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dist(0.0, 1.0);

double random() { return dist(gen); }
