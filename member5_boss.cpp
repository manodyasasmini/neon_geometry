#include "member5_boss.h"
#include "game_engine.h"
#include <cmath>

namespace Member5_Boss {
    void shoot() {
        int patterns = SharedState::level * 4 + 4;
        for (int i = 0; i < patterns; i++) {
            Projectile p;
            p.x = SharedState::bossX; p.y = SharedState::bossY;
            float angle = (i * (360 / patterns)) * 3.14159f / 180.0f;
            p.dx = cos(angle) * (0.02f + SharedState::level * 0.005f);
            p.dy = sin(angle) * (0.02f + SharedState::level * 0.005f);
            p.active = true; p.isEnemyWeapon = true;
            SharedState::projectiles.push_back(p);
        }
    }

    void update() {
        if (SharedState::bossActive) {
            SharedState::bossX = sin(glutGet(GLUT_ELAPSED_TIME) * (0.001f * SharedState::level)) * 0.6f;
            if (++SharedState::bossAttackTimer > (120 - SharedState::level * 20)) {
                shoot();
                SharedState::bossAttackTimer = 0;
            }
        }
    }

    void draw() {
        float t = glutGet(GLUT_ELAPSED_TIME) * 0.05f;
        glPushMatrix();
        glTranslatef(SharedState::bossX, SharedState::bossY, 0);

        if (SharedState::level == 1) glColor3f(1, 0, 1);
        else if (SharedState::level == 2) glColor3f(1, 0.5f, 0);
        else glColor3f(1, 0, 0);

        glRotatef(t, 0, 0, 1);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 8; i++) {
            float a = i * 6.28318f / 8;
            glVertex2f(cos(a) * 0.2f, sin(a) * 0.2f);
        }
        glEnd();

        glRotatef(-2 * t, 0, 0, 1);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.1f, -0.1f); glVertex2f(0.1f, -0.1f);
        glVertex2f(0.1f, 0.1f);   glVertex2f(-0.1f, 0.1f);
        glEnd();
        glPopMatrix();
    }
}
