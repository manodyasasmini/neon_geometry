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
            p.dx = cos(angle) * (0.008f + SharedState::level * 0.002f);
            p.dy = sin(angle) * (0.008f + SharedState::level * 0.002f);
            p.active = true; p.isEnemyWeapon = true;
            SharedState::projectiles.push_back(p);
        }
    }


 // UPDATE LOGIC
    void update() {
        if (SharedState::bossActive) {
            //? Update internal timer for animations
            localTime += 0.016f;

            // Boss Movement (from original project, slowed down for smooth majesty)
            SharedState::bossX = sin(glutGet(GLUT_ELAPSED_TIME) * (0.0004f * SharedState::level)) * 0.6f;

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
 // DRAW LOGIC (Anomaly Boss Geometry)
    void draw() {
        glPushMatrix();
            glTranslatef(SharedState::bossX, SharedState::bossY, 0.0f);

            //  JAGGED SPIKES (each scaled independently)
            for (int i = 0; i < 8; i++) {
                glPushMatrix();
                    glRotatef(i * 45.0f + localTime * 15.0f, 0, 0, 1);
                    glTranslatef(0.0f, 0.18f, 0.0f);

                    // Non-uniform scaling per spike - makes them feel unstable
                    glScalef(1.0f + shearX * 0.5f, spikes[i], 1.0f);

                    glColor3f(1.0f, 0.3f, 0.6f);
                    glBegin(GL_TRIANGLES);
                        glVertex2f(-0.028f, 0.0f);
                        glVertex2f( 0.028f, 0.0f);
                        glVertex2f( 0.0f,   0.10f);
                    glEnd();

                    // Spike outline glow
                    glColor3f(1.0f, 0.7f, 0.9f);
                    glLineWidth(1.5f);
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(-0.028f, 0.0f);
                        glVertex2f( 0.028f, 0.0f);
                        glVertex2f( 0.0f,   0.10f);
                    glEnd();
                glPopMatrix();
            }

            // MAIN CORE - THE STAR OF THE SHOW
            glPushMatrix();
                glRotatef(rotation, 0, 0, 1);

                // STEP 1: Non-uniform scaling (X and Y differ -> stretches)
                glScalef(scaleX, scaleY, 1.0f);

                // STEP 2: SHEARING - the key transformation
                applyShear(shearX, shearY);

                // The square body
                drawSquare(0.12f, 0.85f, 0.1f, 0.4f);
                drawSquareOutline(0.12f, 1.0f, 0.6f, 0.9f, 2.5f);

                // Inner pulsing core
                float corePulse = 0.5f + 0.5f * sin(localTime * 6.0f);
                drawSquare(0.02f + 0.03f * corePulse, 1.0f, 1.0f, 1.0f);

                // Cross-hairs inside (extra detail showing shear effect)
                glColor3f(1.0f, 1.0f, 1.0f);
                glLineWidth(1.5f);
                glBegin(GL_LINES);
                    glVertex2f(-0.12f, 0.0f); glVertex2f(0.12f, 0.0f);
                    glVertex2f(0.0f, -0.12f); glVertex2f(0.0f, 0.12f);
                glEnd();
            glPopMatrix();

        glPopMatrix();
    }
}
