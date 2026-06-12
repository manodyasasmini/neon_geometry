#include "member1_player.h"
#include "game_engine.h"
#include <cmath>

namespace Member1_Player
{
    float x = 0.0f, y = 0.0f;
    float angle = 0.0f;
    float thrusterScale = 1.0f;
    float pulseDir = 0.05f;

    void update()
    {
        // Calculate orientation looking toward crosshair
        angle = atan2(SharedState::mouseY - y, SharedState::mouseX - x) * 180.0f / 3.14159f - 90.0f;

        // Thruster oscillation
        thrusterScale += pulseDir;
        if (thrusterScale > 1.5f || thrusterScale < 0.8f)
            pulseDir = -pulseDir;

        // Broadcast positions to other engine modules
        SharedState::player.x = x;
        SharedState::player.y = y;
        SharedState::player.angle = angle;
    }

    void draw()
    {
        glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);

        // Core hull
        glColor3f(0.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.05f);
        glVertex2f(-0.03f, -0.03f);
        glVertex2f(0.03f, -0.03f);
        glEnd();

        // Localized matrix scaling for dynamic exhaust tail
        glPushMatrix();
        glTranslatef(0.0f, -0.03f, 0.0f);
        glScalef(1.0f, thrusterScale, 1.0f);
        glColor3f(1.0f, 0.5f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-0.015f, -0.03f);
        glVertex2f(0.015f, -0.03f);
        glEnd();
        glPopMatrix();

        glPopMatrix();
    }
}