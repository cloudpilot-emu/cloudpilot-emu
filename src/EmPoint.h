#ifndef _EM_POINT_H_
#define _EM_POINT_H_

#include "EmCommon.h"

class EmPoint {
   public:
    EmPoint() = default;

    EmPoint(const EmPoint&) = default;

    EmPoint(uint32 x, uint32 y);

    void Offset(uint32 x, uint32 y);

    EmPoint operator+(const EmPoint&) const;
    EmPoint operator-(const EmPoint&) const;
    EmPoint operator*(const EmPoint&) const;
    EmPoint operator/(const EmPoint&) const;

    EmPoint& operator=(const EmPoint&) = default;
    EmPoint& operator+=(const EmPoint&);
    EmPoint& operator-=(const EmPoint&);
    EmPoint& operator*=(const EmPoint&);
    EmPoint& operator/=(const EmPoint&);

    EmPoint operator-() const;

    bool operator==(const EmPoint&) const;
    bool operator!=(const EmPoint&) const;

    uint32 x{0}, y{0};
};

#endif  // _EM_POINT_H
