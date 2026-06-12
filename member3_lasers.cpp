#include "member3_lasers.h"
#include "game_engine.h"
#include <cmath>
#include <algorithm>

namespace Member3_Lasers {
    void drawDDA_Line(float x0, float y0, float x1, float y1) {
        float dx = x1 - x0, dy = y1 - y0;
        float steps = std::max(std::abs(dx), std::abs(dy)) * 120.0f;
        if (steps < 1.0f) steps = 1.0f;
        float xInc = dx / steps, yInc = dy / steps;
        float cx = x0, cy = y0;

        glBegin(GL_POINTS);
        for (int i = 0; i <= steps; i++) {
            glVertex2f(cx, cy);
            cx += xInc; cy += yInc;
        }
        glEnd();
    }

    void fire() {
        float dx = SharedState::mouseGL_X - SharedState::playerX;
        float dy = SharedState::mouseGL_Y - SharedState::playerY;
        float len = sqrt(dx * dx + dy * dy);
        if (len > 0) {
            Projectile p; p.x = SharedState::playerX; p.y = SharedState::playerY;
            p.dx = (dx / len) * 0.1f; p.dy = (dy / len) * 0.1f;
            p.active = true; p.isEnemyWeapon = false;
            SharedState::projectiles.push_back(p);
        }
    }

    void update() {
        for (auto& p : SharedState::projectiles) {
            if (!p.active) continue;
            p.x += p.dx; p.y += p.dy;
            if (p.x < -1.1f || p.x > 1.1f || p.y < -1.1f || p.y > 1.1f) p.active = false;
        }
    }

    void draw() {
        glPointSize(2.5f);
        for (const auto& p : projectiles) {
            if (p.active) {
                if (p.isEnemyWeapon) glColor3f(1.0f, 0.0f, 1.0f);
                else glColor3f(1.0f, 1.0f, 0.0f);
                drawDDA_Line(p.x, p.y, p.x - p.dx * 0.5f, p.y - p.dy * 0.5f);
            }
        }
        glPointSize(1.0f);
    }
}
