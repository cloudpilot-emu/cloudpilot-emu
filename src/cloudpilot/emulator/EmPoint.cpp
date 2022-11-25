#include "EmPoint.h"

EmPoint::EmPoint(uint32 x, uint32 y) : x(x), y(y) {}

void EmPoint::Offset(uint32 x, uint32 y) {
    this->x += x;
    this->y += y;
}

EmPoint EmPoint::operator+(const EmPoint& p) const { return EmPoint(x + p.x, y + p.y); }
EmPoint EmPoint::operator-(const EmPoint& p) const { return EmPoint(x - p.x, y - p.y); }
EmPoint EmPoint::operator*(const EmPoint& p) const { return EmPoint(x * p.x, y * p.y); }
EmPoint EmPoint::operator/(const EmPoint& p) const { return EmPoint(x / p.x, y / p.y); }

EmPoint& EmPoint::operator+=(const EmPoint& p) {
    x += p.x;
    y += p.y;

    return *this;
}

EmPoint& EmPoint::operator-=(const EmPoint& p) {
    x -= p.x;
    y -= p.y;

    return *this;
}

EmPoint& EmPoint::operator*=(const EmPoint& p) {
    x *= p.x;
    y *= p.y;

    return *this;
}

EmPoint& EmPoint::operator/=(const EmPoint& p) {
    x /= p.x;
    y /= p.y;

    return *this;
}

EmPoint EmPoint::operator-() const { return EmPoint(-x, -y); }

bool EmPoint::operator==(const EmPoint& p) const { return x == p.x && y == p.y; }
bool EmPoint::operator!=(const EmPoint& p) const { return x != p.x || y != p.y; }
