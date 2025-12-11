#pragma once

struct Point {
    int x, y;
    
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    
    double distanceTo(const Point& other) const;
    
    bool isValid() const {
        return x >= 0 && x <= 500 && y >= 0 && y <= 500;
    }
};
