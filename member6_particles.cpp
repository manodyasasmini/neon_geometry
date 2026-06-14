#include "member6_particles.h"
#include "game_engine.h"
#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>

namespace Member6_Particles {
    // Member 6 manages their own list of active particles
    std::vector<Particle> activeParticles;

    void trigger(float x, float y) {
        // Create an explosive burst of 15 pieces of shrapnel
        for (int i = 0; i < 15; i++) {
            Particle p;
            p.x = x;
            p.y = y;

            // 1. TRIGONOMETRY MATH: Calculate a random outward angle in radians
            float angle = (rand() % 360) * 3.14159f / 180.0f;

            // Random explosion force (speed)
            float speed = (rand() % 50 + 20) * 0.001f;

            // 2. RADIAL VECTOR: Convert angle and speed to X/Y velocity
            p.dx = cos(angle) * speed;
            p.dy = sin(angle) * speed;

            p.life = 1.0f; // Starts at 100% life

            // Randomize fire colors (White to Yellow to Orange)
            p.r = 1.0f;
            p.g = (rand() % 80 + 20) / 100.0f; // Green channel varies to create yellow/orange
            p.b = 0.0f;

            activeParticles.push_back(p);
        }
    }

    void update() {
        for (auto& p : activeParticles) {
            // Apply Translation (Velocity)
            p.x += p.dx;
            p.y += p.dy;

            // 3. KINEMATIC FRICTION: Shrapnel slows down over time due to "air resistance"
            p.dx *= 0.92f;
            p.dy *= 0.92f;

            // Decay life
            p.life -= 0.025f;
        }

        // Cleanup dead particles
        activeParticles.erase(std::remove_if(activeParticles.begin(), activeParticles.end(),
            [](const Particle& p){ return p.life <= 0.0f; }), activeParticles.end());
    }

    void draw() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (const auto& p : activeParticles) {
            glPushMatrix(); // Begin Composite Transformation

            // A. TRANSLATION: Move to the particle's dynamic position
            glTranslatef(p.x, p.y, 0.0f);

            // B. ROTATION: Tumbling debris effect.
            // It spins wildly at first, then slows down as life approaches 0.
            glRotatef(p.life * 1000.0f, 0.0f, 0.0f, 1.0f);

            // C. SCALING: The shrapnel shrinks down to nothing as it cools off/dies
            glScalef(p.life, p.life, 1.0f);

            // Fading Alpha channel
            glColor4f(p.r, p.g, p.b, p.life);

            // Draw a jagged piece of shrapnel (Triangle) instead of a boring dot
            float s = 0.02f;
            glBegin(GL_TRIANGLES);
            glVertex2f(0.0f, s);
            glVertex2f(-s, -s);
            glVertex2f(s, -s);
            glEnd();

            glPopMatrix(); // End Composite Transformation
        }
        glDisable(GL_BLEND);
    }
}
