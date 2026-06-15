#include "member2_swarm.h"
#include "game_engine.h"
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <algorithm>

namespace Member2_Swarm {

    // =========================================================================
    // LECTURE ALGORITHM 1: DDA Dashed Line Drawing Algorithm
    // =========================================================================
    void drawDDALine(float x0, float y0, float x1, float y1) {
        float dx = x1 - x0;
        float dy = y1 - y0;

        int steps = std::max(std::abs(dx * 1000.0f), std::abs(dy * 1000.0f));

        float xInc = dx / (float)steps;
        float yInc = dy / (float)steps;

        float x = x0;
        float y = y0;

        glBegin(GL_POINTS);
        for (int i = 0; i <= steps; i++) {
            // Only draw pixels in chunks to create a dashed line
            if ((i / 15) % 2 == 0) {
                glVertex2f(x, y);
            }
            x += xInc;
            y += yInc;
        }
        glEnd();
    }

    // =========================================================================
    // LECTURE ALGORITHM 2: Bresenham's Circle Algorithm
    // =========================================================================
    void drawBresenhamCircle(float xc, float yc, float r_float) {
        // Convert float radius to integer space for the strict Bresenham formula
        int r = (int)(r_float * 1000.0f);
        int x = 0;
        int y = r;
        int d = 3 - 2 * r; // Initial decision parameter

        glBegin(GL_POINTS);
        while (x <= y) {
            // Plot 8 symmetric octants, scaling back down to floats for OpenGL
            glVertex2f(xc + x/1000.0f, yc + y/1000.0f);
            glVertex2f(xc - x/1000.0f, yc + y/1000.0f);
            glVertex2f(xc + x/1000.0f, yc - y/1000.0f);
            glVertex2f(xc - x/1000.0f, yc - y/1000.0f);
            glVertex2f(xc + y/1000.0f, yc + x/1000.0f);
            glVertex2f(xc - y/1000.0f, yc + x/1000.0f);
            glVertex2f(xc + y/1000.0f, yc - x/1000.0f);
            glVertex2f(xc - y/1000.0f, yc - x/1000.0f);

            // Decision rules
            if (d < 0) {
                d = d + 4 * x + 6;
            } else {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
        glEnd();
    }

    // =========================================================================
    // SWARM LOGIC
    // =========================================================================
    void spawnWave() {
        SharedState::enemies.clear();
        int count = SharedState::wave * (5 + SharedState::level);
        for (int i = 0; i < count; i++) {
            Enemy e;
            e.x = (rand() % 2 == 0 ? 1.2f : -1.2f);
            e.y = ((rand() % 200) / 100.0f) - 1.0f;
            e.spawnProgress = 0.0f;

            if (rand() % 100 < 20) {
                e.type = TANK;
                e.hp = 3;
                e.speed = 0.0006f + (SharedState::level * 0.0002f);
            } else {
                e.type = SWARM;
                e.hp = 1;
                e.speed = 0.0016f + (SharedState::level * 0.0004f);
            }
            SharedState::enemies.push_back(e);
        }
    }

    void update() {
        for (size_t i = 0; i < SharedState::enemies.size(); i++) {
            auto& e = SharedState::enemies[i];
            if (e.x > 2) continue; // Skip dead enemies

            // 1. Warp-in logic
            if (e.spawnProgress < 1.0f) {
                e.spawnProgress += 0.02f;
            }

            // 2. Base Vector to Player
            float dx = SharedState::playerX - e.x;
            float dy = SharedState::playerY - e.y;

            // 3. FLOCKING REPULSION MATH (Prevent overlapping)
            float repulsionX = 0.0f;
            float repulsionY = 0.0f;
            for (size_t j = 0; j < SharedState::enemies.size(); j++) {
                if (i == j || SharedState::enemies[j].x > 2) continue;

                auto& other = SharedState::enemies[j];
                float distX = e.x - other.x;
                float distY = e.y - other.y;
                float dist = sqrt(distX * distX + distY * distY);

                // If they are too close, push them apart
                if (dist > 0.001f && dist < 0.08f) {
                    repulsionX += (distX / dist) * 0.002f; // Push strength
                    repulsionY += (distY / dist) * 0.002f;
                }
            }

            // Add repulsion to the tracking vector
            dx += repulsionX;
            dy += repulsionY;

            // Normalize and Apply Speed
            float len = sqrt(dx * dx + dy * dy) + 0.001f;
            e.x += (dx / len) * e.speed;
            e.y += (dy / len) * e.speed;
        }
    }

    void draw() {
        float time = glutGet(GLUT_ELAPSED_TIME) * 0.001f;

        for (auto& e : SharedState::enemies) {
            if (e.x > 2) continue;

            // --- DDA TETHER LOGIC ---
            if (e.type == SWARM && e.spawnProgress >= 1.0f) {
                float dx = SharedState::playerX - e.x;
                float dy = SharedState::playerY - e.y;
                float distToPlayer = sqrt(dx * dx + dy * dy);

                // PROXIMITY CHECK: Only draw the tether if they are getting close
                if (distToPlayer < 0.4f) {
                    float dangerIntensity = 1.0f - (distToPlayer / 0.4f);
                    glColor4f(1.0f, 0.0f, 0.0f, dangerIntensity * 0.5f); // Faint Red Warning Laser
                    drawDDALine(e.x, e.y, SharedState::playerX, SharedState::playerY);
                }
            }

            glPushMatrix();

            // 1. TRANSLATION
            glTranslatef(e.x, e.y, 0.0f);

            // 2. WARP-IN SCALING (Sine-based Ease-Out algorithm)
            float popScale = sin(e.spawnProgress * 3.14159f / 2.0f);
            glScalef(popScale, popScale, 1.0f);

            if (e.type == TANK) {
                // 3. ROTATION
                glRotatef(time * 40.0f, 0.0f, 0.0f, 1.0f);

                // Dynamic Breathing based on HP
                float healthRatio = e.hp / 3.0f;
                float breathSpeed = 4.0f + (1.0f - healthRatio) * 10.0f;
                float breathScale = 1.0f + 0.15f * sin(time * breathSpeed);
                glScalef(breathScale, breathScale, 1.0f);

                float s = 0.06f;

                // BRESENHAM CIRCLE ALGORITHM: Draw the outer energy shield
                glColor4f(healthRatio, 0.0f, healthRatio * 0.2f, 0.8f);
                glPointSize(2.0f);
                drawBresenhamCircle(0.0f, 0.0f, s * 1.5f);
                glPointSize(1.0f);

                // Core polygon
                glColor4f(healthRatio, healthRatio * 0.2f, healthRatio * 0.2f, 1.0f);
                glBegin(GL_QUADS);
                glVertex2f(0, s); glVertex2f(-s, 0);
                glVertex2f(0, -s); glVertex2f(s, 0);
                glEnd();

            } else {
                float dx = SharedState::playerX - e.x;
                float dy = SharedState::playerY - e.y;
                float len = sqrt(dx * dx + dy * dy) + 0.001f;

                // 4. SHEARING (Affine Matrix)
                float shearAmount = 0.6f * e.spawnProgress;
                GLfloat shearMatrix[16] = {
                    1.0f,                   (dy/len) * shearAmount, 0.0f, 0.0f,
                    (dx/len) * shearAmount, 1.0f,                   0.0f, 0.0f,
                    0.0f,                   0.0f,                   1.0f, 0.0f,
                    0.0f,                   0.0f,                   0.0f, 1.0f
                };
                glMultMatrixf(shearMatrix);

                // ROTATION
                glRotatef(time * 200.0f, 0.0f, 0.0f, 1.0f);

                float s = 0.035f;

                // Outer Glow
                glColor4f(1.0f, 0.5f, 0.0f, 0.3f);
                glBegin(GL_TRIANGLES);
                glVertex2f(0.0f, s * 1.5f);
                glVertex2f(-s * 1.5f, -s * 1.5f); glVertex2f(s * 1.5f, -s * 1.5f);
                glEnd();

                // Core
                glColor4f(1.0f, 0.8f, 0.0f, 1.0f);
                glBegin(GL_TRIANGLES);
                glVertex2f(0.0f, s);
                glVertex2f(-s, -s); glVertex2f(s, -s);
                glEnd();
            }
            glPopMatrix();
        }
    }
}
