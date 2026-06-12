#ifndef PARTICLES_H
#define PARTICLES_H

namespace Member6_Particles {
    struct Burst { float x, y, life; };
    void trigger(float x, float y);
    void update();
    void draw();
}

#endif
