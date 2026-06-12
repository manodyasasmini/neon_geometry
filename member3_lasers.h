#ifndef LASERS_H
#define LASERS_H

#include <vector>

namespace Member3_Lasers
{
    struct Laser
    {
        float x, y;
        float dx, dy;
        bool active;
    };
    extern std::vector<Laser> pool;

    void fire();
    void update();
    void draw();
}

#endif
