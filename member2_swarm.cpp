#include "member2_swarm.h"
#include "game_engine.h"
#include <cmath>
#include <cstdlib>

namespace Member2_Swarm
{
    std::vector<Enemy> pool;

    void spawn()
    {
        Enemy e;
        e.x = (rand() % 2 == 0) ? 1.1f : -1.1f;
        e.y = ((rand() % 200) / 100.0f) - 1.0f;
        e.speed = 0.002f;
        e.active = true;
        pool.push_back(e);
    }

    void update()
    {
        if (pool.size() < 4)
            spawn();

        for (auto &e : pool)
        {
            if (!e.active)
                continue;
            float dx = SharedState::player.x - e.x;
            float dy = SharedState::player.y - e.y;
            float len = sqrt(dx * dx + dy * dy);
            if (len > 0.01f)
            {
                e.x += (dx / len) * e.speed;
                e.y += (dy / len) * e.speed;
            }
        }
    }

    void draw()
    {
        glColor3f(1.0f, 0.2f, 0.2f);
        for (const auto &e : pool)
        {
            if (!e.active)
                continue;
            glPushMatrix();
            glTranslatef(e.x, e.y, 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-0.03f, -0.03f);
            glVertex2f(0.03f, -0.03f);
            glVertex2f(0.03f, 0.03f);
            glVertex2f(-0.03f, 0.03f);
            glEnd();
            glPopMatrix();
        }
    }
}