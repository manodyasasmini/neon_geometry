#include "member2_swarm.h"
#include "game_engine.h"
#include <cmath>
#include <cstdlib>

namespace Member2_Swarm {
    void spawnWave() {
        SharedState::enemies.clear();
        int count = SharedState::wave * (5 + SharedState::level);
        for (int i = 0; i < count; i++) {
            Enemy e;
            e.x = (rand() % 2 == 0 ? 1.2f : -1.2f);
            e.y = ((rand() % 200) / 100.0f) - 1.0f;

            if (rand() % 100 < 20) {
                e.type = TANK;
                e.hp = 3;
                e.speed = 0.0006f + (SharedState::level * 0.0002f);
            } else {
                e.type = SWARM;
                e.hp = 1;
                e.speed = 0.0016f + (SharedState::level * 0.0004f);
            }
            SharedState::enemies.push_back(e);
        }
    }

    void update() {
        for (auto& e : SharedState::enemies) {
            if (e.x > 2) continue;
            float dx = SharedState::playerX - e.x, dy = SharedState::playerY - e.y;
            float len = sqrt(dx * dx + dy * dy) + 0.001f;
            e.x += (dx / len) * e.speed; e.y += (dy / len) * e.speed;
        }
    }

    void draw() {
        for (auto& e : SharedState::enemies) {
            if (e.x > 2) continue;
            if (e.type == TANK) glColor3f(1.0f, 0.0f, 0.0f);
            else glColor3f(1.0f, 0.5f, 0.0f);

            float s = (e.type == TANK) ? 0.08f : 0.04f;
            glPushMatrix();
            glTranslatef(e.x, e.y, 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-s/2, -s/2); glVertex2f(s/2, -s/2);
            glVertex2f(s/2, s/2);   glVertex2f(-s/2, s/2);
            glEnd();
            glPopMatrix();
        }
    }
}
