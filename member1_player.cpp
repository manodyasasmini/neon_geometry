#include "member1_player.h"
#include "game_engine.h"
#include <cmath>

namespace Member1_Player {
    float thrusterScale = 1.0f;
    float pulseDir = 0.05f;

    void update() {
        if (SharedState::gameState == PLAYING) {
            SharedState::playerAngle = atan2(SharedState::mouseGL_Y - SharedState::playerY, SharedState::mouseGL_X - SharedState::playerX) * 180.0f / 3.14159f - 90.0f;
        }

        thrusterScale += pulseDir;
        if (thrusterScale > 1.5f || thrusterScale < 0.8f) pulseDir = -pulseDir;
    }

    void draw() {
        glPushMatrix();
        glTranslatef(SharedState::playerX, SharedState::playerY, 0);
        glRotatef(SharedState::playerAngle, 0, 0, 1);

        if (SharedState::invulnTimer > 0 && SharedState::invulnTimer % 10 < 5) glColor3f(1.0f, 0.0f, 0.0f);
        else glColor3f(0.0f, 1.0f, 1.0f);

        glBegin(GL_TRIANGLES);
        glVertex2f(0, 0.05f);
        glVertex2f(-0.03f, -0.03f);
        glVertex2f(0.03f, -0.03f);
        glEnd();

        if (SharedState::isDashing) {
            glPushMatrix();
            glTranslatef(0.0f, -0.03f, 0.0f);
            glScalef(1.0f, thrusterScale, 1.0f);
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_TRIANGLES);
            glVertex2f(0, 0.0f);
            glVertex2f(-0.02f, -0.05f);
            glVertex2f(0.02f, -0.05f);
            glEnd();
            glPopMatrix();
        }
        glPopMatrix();
    }
}
