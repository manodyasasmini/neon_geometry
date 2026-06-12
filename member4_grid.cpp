#include "member4_grid.h"
#include "game_engine.h"
#include <cmath>

namespace Member4_Grid
{
    float heartbeat = 1.0f;
    float pulseTime = 0.0f;

    void update()
    {
        pulseTime += 0.03f;
        heartbeat = 1.0f + 0.04f * sin(pulseTime);
    }

    void draw()
    {
        glPushMatrix();

        // Custom shearing matrix mapping applied directly to current matrix stack
        float shearX = -SharedState::player.x * 0.1f;
        float shearY = -SharedState::player.y * 0.1f;
        float shearMatrix[16] = {
            1.0f, shearY, 0.0f, 0.0f,
            shearX, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};
        glMultMatrixf(shearMatrix);
        glScalef(heartbeat, heartbeat, 1.0f);

        glColor3f(0.0f, 0.15f, 0.4f);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        for (float i = -1.0f; i <= 1.0f; i += 0.2f)
        {
            glVertex2f(i, -1.0f);
            glVertex2f(i, 1.0f);
            glVertex2f(-1.0f, i);
            glVertex2f(1.0f, i);
        }
        glEnd();
        glPopMatrix();
    }
}