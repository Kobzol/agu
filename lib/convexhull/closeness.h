#pragma once

#include <vector>

#include "../common.h"

std::vector<Point> giftWrap(std::vector<Point> points);
std::vector<Point> grahamScan(std::vector<Point> points);
std::vector<Point> quickHull(std::vector<Point> points);
std::vector<Point> divideAndConquer(std::vector<Point> points);
