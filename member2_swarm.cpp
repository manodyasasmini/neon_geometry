#include "member2_swarm.h"
#include "game_engine.h"
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

namespace Member2_Swarm {
    void spawnWave() {
        SharedState::enemies.clear();
        int count = SharedState::wave * (5 + SharedState::level);
        for (int i = 0; i < count; i++) {
            Enemy e;
            e.x = (rand() % 2 == 0 ? 1.2f : -1.2f);
            e.y = ((rand() % 200) / 100.0f) - 1.0f;
            e.spawnProgress = 0.0f; // NEW: Starts at 0 for warp-in animation

            if (rand() % 100 < 20) {
                e.type = TANK;
                e.hp = 3;
                e.speed = 0.001f + (SharedState::level * 0.0005f);
            } else {
                e.type = SWARM;
                e.hp = 1;
                e.speed = 0.003f + (SharedState::level * 0.001f);
            }
            SharedState::enemies.push_back(e);
        }
    }

    void update() {
        for (size_t i = 0; i < SharedState::enemies.size(); i++) {
            auto& e = SharedState::enemies[i];
            if (e.x > 2) continue;

            // 1. Warp-in logic (takes about 60 frames to fully spawn)
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

            glPushMatrix();

            // TRANSLATION
            glTranslatef(e.x, e.y, 0.0f);

            // WARP-IN SCALING (Spawns tiny and grows to full size)
            // Use a smoothstep-like curve for a "pop" effect
            float popScale = sin(e.spawnProgress * 3.14159f / 2.0f);
            glScalef(popScale, popScale, 1.0f);

            if (e.type == TANK) {
                glRotatef(time * 40.0f, 0.0f, 0.0f, 1.0f);

                // Dynamic Breathing based on HP (Faster when damaged)
                float healthRatio = e.hp / 3.0f;
                float breathSpeed = 4.0f + (1.0f - healthRatio) * 10.0f;
                float breathScale = 1.0f + 0.15f * sin(time * breathSpeed);
                glScalef(breathScale, breathScale, 1.0f);

                float s = 0.06f;

                // Dynamic Color based on HP (Bright Red -> Dark Maroon)
                glColor4f(healthRatio, 0.0f, healthRatio * 0.2f, 0.3f); // Glow
                glBegin(GL_QUADS);
                glVertex2f(0, s * 1.5f); glVertex2f(-s * 1.5f, 0);
                glVertex2f(0, -s * 1.5f); glVertex2f(s * 1.5f, 0);
                glEnd();

                glColor4f(healthRatio, healthRatio * 0.2f, healthRatio * 0.2f, 1.0f); // Core
                glBegin(GL_QUADS);
                glVertex2f(0, s); glVertex2f(-s, 0);
                glVertex2f(0, -s); glVertex2f(s, 0);
                glEnd();

            } else {
                // Swarmer Shearing
                float dx = SharedState::playerX - e.x;
                float dy = SharedState::playerY - e.y;
                float len = sqrt(dx * dx + dy * dy) + 0.001f;

                float shearAmount = 0.6f * e.spawnProgress; // Only shear once spawned
                GLfloat shearMatrix[16] = {
                    1.0f,                 (dy/len) * shearAmount, 0.0f, 0.0f,
                    (dx/len) * shearAmount, 1.0f,                 0.0f, 0.0f,
                    0.0f,                 0.0f,                 1.0f, 0.0f,
                    0.0f,                 0.0f,                 0.0f, 1.0f
                };
                glMultMatrixf(shearMatrix);

                // Spin
                glRotatef(time * 200.0f, 0.0f, 0.0f, 1.0f);

                float s = 0.035f;

                glColor4f(1.0f, 0.5f, 0.0f, 0.3f);
                glBegin(GL_TRIANGLES);
                glVertex2f(0.0f, s * 1.5f);
                glVertex2f(-s * 1.5f, -s * 1.5f); glVertex2f(s * 1.5f, -s * 1.5f);
                glEnd();

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
