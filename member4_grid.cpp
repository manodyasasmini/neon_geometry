#include "member4_grid.h"
#include "game_engine.h"
#include <cmath>

namespace Member4_Grid {
    float pulseTime = 0.0f;
    float scaleMod = 1.0f;

    void update() {
        pulseTime += 0.03f;
        scaleMod = 1.0f + 0.05f * sin(pulseTime);
    }

    void draw() {
        glPushMatrix();
        float shearX = -SharedState::playerX * 0.08f;
        float shearMatrix[16] = {
            1.0f,   0.0f,   0.0f, 0.0f,
            shearX, 1.0f,   0.0f, 0.0f,
            0.0f,   0.0f,   1.0f, 0.0f,
            0.0f,   0.0f,   0.0f, 1.0f
        };
        glMultMatrixf(shearMatrix);
        glScalef(scaleMod, scaleMod, 1.0f);

        glColor3f(0, 0.3f + (SharedState::level * 0.1f), 0.7f - (SharedState::level * 0.15f));
        for (int r = 1; r <= 8; r++) {
            float radius = r * 0.1f;
            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 60; i++) {
                float a = i * 6.28318f / 60;
                glVertex2f(cos(a) * radius, sin(a) * radius);
            }
            glEnd();
        }
        glPopMatrix();
    }
}
