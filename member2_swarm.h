#ifndef SWARM_H
#define SWARM_H


#include <vector>

namespace Member2_Swarm
{
    struct Enemy
    {
        float x, y;
        float speed;
        bool active;
    };
    extern std::vector<Enemy> pool;

    void spawn();
    void update();
    void draw();
}

#endif
