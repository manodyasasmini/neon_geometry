#include "member6_particles.h"
#include "game_engine.h"
#include <cmath>
#include <cstdlib>

namespace Member6_Particles
{
    std::vector<Particle> pool;

    void triggerExplosion(float x, float y)
    {
        for (int i = 0; i < 15; i++)
        {
            Particle p;
            p.x = x;
            p.y = y;
            float angle = (rand() % 360) * 3.14159f / 180.0f;
            float speed = ((rand() % 100) / 100.0f) * 0.02f + 0.01f;
            p.vx = cos(angle) * speed;
            p.vy = sin(angle) * speed;
            p.life = 1.0f;
            pool.push_back(p);
        }
    }

    void update()
    {
        for (auto &p : pool)
        {
            if (p.life <= 0.0f)
                continue;
            p.x += p.vx;
            p.y += p.vy;
            p.life -= 0.04f;
        }
    }

    void draw()
    {
        glBegin(GL_POINTS);
        for (const auto &p : pool)
        {
            if (p.life <= 0.0f)
                continue;
            glColor3f(1.0f * p.life, 0.6f * p.life, 0.0f);
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }
}