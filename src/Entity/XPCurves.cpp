#include "Entity/XPCurves.hpp"

using namespace Pokemon; 

int XPCurve::xpForLevel(int n) const {
    switch (this->group) {
        case XPGroup::MediumFast:
            return n * n * n;

        case XPGroup::Erratic:
            if (n < 50)  return (n*n*n * (100 - n)) / 50;
            if (n < 68)  return (n*n*n * (150 - n)) / 100;
            if (n < 98)  return (n*n*n * ((1911 - 10*n) / 3)) / 500;
            else         return (n*n*n * (160 - n)) / 100;

        case XPGroup::Fluctuating:
            if (n < 15)  return (n*n*n * ((n + 1) / 3 + 24)) / 50;
            if (n < 36)  return (n*n*n * (n + 14)) / 50;
            else         return (n*n*n * (n / 2 + 32)) / 50;

        case XPGroup::MediumSlow:
            return (6*n*n*n/5) - (15*n*n) + (100*n) - 140;

        case XPGroup::Fast:
            return (4 * n*n*n) / 5;

        case XPGroup::Slow:
            return (5 * n*n*n) / 4;
        
        default :
            return 0;
    }
}

int XPCurve::xpToNextLevel(int lvl)const {
    return xpForLevel(lvl + 1);
}