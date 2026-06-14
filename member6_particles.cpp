#include "member6_particles.h"
#include "game_engine.h"
#include <vector>
#include <cstdlib>
#include <algorithm>

namespace Member6_Particles {
    std::vector<Burst> activeBursts;

    void trigger(float x, float y) {
        Burst b = {x, y, 1.0f};
        activeBursts.push_back(b);
    }

    void update() {
        for (auto& b : activeBursts) {
            b.life -= 0.05f;
        }
        activeBursts.erase(std::remove_if(activeBursts.begin(), activeBursts.end(),
            [](const Burst& b){ return b.life <= 0.0f; }), activeBursts.end());
    }

    void draw() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPointSize(4.0f);
        for (const auto& b : activeBursts) {
            glColor4f(1.0f, 0.6f, 0.1f, b.life);
            glBegin(GL_POINTS);
            for(int i=0; i<8; i++) {
                glVertex2f(b.x + ((rand()%10-5)*0.01f), b.y + ((rand()%10-5)*0.01f));
            }
            glEnd();
        }
        glPointSize(1.0f);
        glDisable(GL_BLEND);
    }
}
