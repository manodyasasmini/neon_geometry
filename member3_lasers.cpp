#include "member3_lasers.h"
#include "game_engine.h"
#include <cmath>
#include <algorithm>

namespace Member3_Lasers
{
    std::vector<Laser> pool;

    // Academic Implementation: DDA Line Drawing Algorithm rasterization
    void drawCustomLine(float x0, float y0, float x1, float y1)
    {
        float dx = x1 - x0;
        float dy = y1 - y0;
        float steps = std::max(std::abs(dx), std::abs(dy)) * 100.0f;
        if (steps < 1.0f)
            steps = 1.0f;

        float xInc = dx / steps;
        float yInc = dy / steps;
        float cx = x0;
        float cy = y0;

        glBegin(GL_POINTS);
        for (int i = 0; i <= steps; i++)
        {
            glVertex2f(cx, cy);
            cx += xInc;
            cy += yInc;
        }
        glEnd();
    }

    void fire()
    {
        float targetDx = SharedState::mouseX - SharedState::player.x;
        float targetDy = SharedState::mouseY - SharedState::player.y;
        float len = sqrt(targetDx * targetDx + targetDy * targetDy);
        if (len > 0)
        {
            Laser l;
            l.x = SharedState::player.x;
            l.y = SharedState::player.y;
            l.dx = (targetDx / len) * 0.04f;
            l.dy = (targetDy / len) * 0.04f;
            l.active = true;
            pool.push_back(l);
        }
    }

    void update()
    {
        for (auto &l : pool)
        {
            if (!l.active)
                continue;
            l.x += l.dx;
            l.y += l.dy;
            if (std::abs(l.x) > 1.2f || std::abs(l.y) > 1.2f)
                l.active = false;
        }
    }

    void draw()
    {
        glColor3f(1.0f, 1.0f, 0.0f);
        glPointSize(3.0f);
        for (const auto &l : pool)
        {
            if (!l.active)
                continue;
            drawCustomLine(l.x, l.y, l.x - l.dx * 1.5f, l.y - l.dy * 1.5f);
        }
        glPointSize(1.0f);
    }
}