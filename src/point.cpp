#include <cmath>

#include "point.h"

double Point::distanceTo(const Point& other) const {
    int dx = x - other.x;
    int dy = y - other.y;
    return std::sqrt(dx*dx + dy*dy);
}