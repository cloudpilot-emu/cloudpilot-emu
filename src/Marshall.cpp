#include "EmPalmStructs.h"
#include "Marshal.h"

void Marshal::GetPointType(emuptr p, PointType& dest) {
    memset(&dest, 0, sizeof(PointType));

    if (p) {
        EmAliasPointType<PAS> src(p);

        dest.x = src.x;
        dest.y = src.y;
    }
}

void Marshal::PutPointType(emuptr p, const PointType& src) {
    if (p) {
        EmAliasPointType<PAS> dest(p);

        dest.x = src.x;
        dest.y = src.y;
    }
}
