#include "member5_boss.h"
#include "game_engine.h"
#include <cmath>

namespace Member5_Boss {
    //  INTERNAL ANIMATION STATE
    float localTime = 0.0f;
    float rotation = 0.0f;
    float shearX = 0.0f, shearY = 0.0f;
    float scaleX = 1.0f, scaleY = 1.0f;
    float spikes[8] = {1, 1, 1, 1, 1, 1, 1, 1};

 // CUSTOM SHEAR MATRIX
    void applyShear(float shx, float shy) {
        float m[16] = {
            1.0f, shy,  0.0f, 0.0f,   // column 1
            shx,  1.0f, 0.0f, 0.0f,   // column 2
            0.0f, 0.0f, 1.0f, 0.0f,   // column 3
            0.0f, 0.0f, 0.0f, 1.0f    // column 4
        };
        glMultMatrixf(m);
    }

    // PRIMITIVE: SQUARE
    void drawSquare(float s, float r, float g, float b) {
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
            glVertex2f(-s, -s);
            glVertex2f( s, -s);
            glVertex2f( s,  s);
            glVertex2f(-s,  s);
        glEnd();
    }

    void drawSquareOutline(float s, float r, float g, float b, float lw) {
        glColor3f(r, g, b);
        glLineWidth(lw);
        glBegin(GL_LINE_LOOP);
            glVertex2f(-s, -s);
            glVertex2f( s, -s);
            glVertex2f( s,  s);
            glVertex2f(-s,  s);
        glEnd();
    }

    // SHOOT LOGIC (Kept from original project)
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



 // UPDATE LOGIC
    void update() {
        if (SharedState::bossActive) {
            //? Update internal timer for animations
            localTime += 0.016f;

            // Boss Movement (from original project)
            SharedState::bossX = sin(glutGet(GLUT_ELAPSED_TIME) * (0.001f * SharedState::level)) * 0.6f;

            // Attack logic (from original project)
            if (++SharedState::bossAttackTimer > (120 - SharedState::level * 20)) {
                shoot();
                SharedState::bossAttackTimer = 0;
            }

            // Anomaly Boss Geometry Math
            shearX = sin(localTime * 1.4f) * 0.45f;
            shearY = cos(localTime * 1.1f) * 0.45f;

            scaleX = 1.0f + 0.18f * sin(localTime * 2.0f);
            scaleY = 1.0f + 0.18f * cos(localTime * 2.6f);

            rotation += 0.4f;

            for (int i = 0; i < 8; i++) {
                spikes[i] = 0.6f + 0.6f * sin(localTime * 3.5f + i * 0.785f);
            }
        }
    }



    }

