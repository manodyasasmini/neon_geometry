#ifndef PARTICLES_H
#define PARTICLES_H

#include <vector>

namespace Member6_Particles
{
    struct Particle
    {
        float x, y;
        float vx, vy;
        float life;
    };
    extern std::vector<Particle> pool;

    void triggerExplosion(float x, float y);
    void update();
    void draw();
}

#endif
