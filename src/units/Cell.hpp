#pragma once

namespace sw {

struct Cell {
    Cell(int x, int y)
        : x{x}
        , y{y}
    {}
    int x;
    int y;
};

inline bool operator==(const Cell& lhs, const Cell& rhs) {
    return lhs.x == rhs.x &&
           lhs.y == rhs.y;
}

inline bool operator!=(const Cell& lhs, const Cell& rhs) {
    return !(lhs == rhs);
}

inline Cell& operator+=(Cell& lhs, const Cell& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

inline Cell operator+(const Cell& lhs, const Cell& rhs) {
    Cell temp{lhs};
    return temp += rhs;
}

inline Cell& operator-=(Cell& lhs, const Cell& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

inline Cell operator-(const Cell& lhs, const Cell& rhs) {
    Cell temp{lhs};
    return temp-=rhs;
}

} // namespace sw
