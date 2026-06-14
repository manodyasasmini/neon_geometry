#include "member4_grid.h"
#include "game_engine.h"
#include <cmath>
#include <algorithm>
#include <chrono>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

namespace Member4_Grid {
    // Animation state variables
    float pulseTime = 0.0f;
    float scaleMod = 1.0f;
    float dynamicShearIntensity = 0.0f; // Interpolated intensity for high-speed warp effects

    // High-resolution timestamp for delta-time calculation
    auto lastTime = std::chrono::steady_clock::now();

    /**
     * Updates the simulation state of the procedural grid arena.
     * Utilizes internal delta-time tracking to ensure frame-rate independent animations.
     */
    void update() {
        // Calculate real-time elapsed between frames
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - lastTime;
        lastTime = currentTime;

        float dt = elapsedTime.count();
        // Fallback constraint to prevent simulation jumps during window dragging or focus loss
        if (dt > 0.1f) dt = 0.016f;

        // Compute periodic scale factor for the organic breathing effect
        pulseTime += 1.8f * dt;
        scaleMod = 1.0f + 0.04f * std::sin(pulseTime);

        // Linear interpolation (Lerp) for mechanical warp attenuation based on player state
        if (SharedState::isDashing) {
            // Rapid attack envelope when acceleration is active
            dynamicShearIntensity = std::min(1.5f, dynamicShearIntensity + 8.0f * dt);
        } else {
            // Smooth decay envelope back to equilibrium under idle conditions
            dynamicShearIntensity = std::max(0.0f, dynamicShearIntensity - 4.0f * dt);
        }
    }

    /**
     * Renders the procedurally generated radial grid arena.
     * Applies compound spatial transformations via a custom column-major shear matrix.
     */
    void draw() {
        // Isolate matrix stack states to protect peripheral engine layers from local transforms
        glPushMatrix();

        // Compute aggregate horizontal shear coefficient
        // Combines static player coordinates with transient kinetic dash forces
        float baseShear = -SharedState::playerX * 0.06f;
        if (SharedState::isDashing) {
            baseShear += SharedState::dashDx * dynamicShearIntensity * 2.0f;
        }

        // Custom 4x4 Homogeneous Transformation Matrix
        // Formatted in Column-Major order to natively match OpenGL specification standards
        float shearMatrix[16] = {
            1.0f,      0.0f, 0.0f, 0.0f,  // Column 1: Maps X components
            baseShear, 1.0f, 0.0f, 0.0f,  // Column 2: Maps Y components (Horizontal Shear)
            0.0f,      0.0f, 1.0f, 0.0f,  // Column 3: Maps Z components
            0.0f,      0.0f, 0.0f, 1.0f   // Column 4: Translation mappings
        };

        // Inject custom geometric transformations into the fixed-function pipeline matrix
        glMultMatrixf(shearMatrix);
        glScalef(scaleMod, scaleMod, 1.0f);

        // Contextual Color Blending Core
        // Dynamically modulates RGB channels based on game level progression and global impact triggers
        float impactGlow = SharedState::screenShake * 3.0f;

        float redChannel   = std::min(1.0f, 0.0f + impactGlow);
        float greenChannel = std::fmod(0.3f + (SharedState::level * 0.05f) + impactGlow, 1.0f);
        float blueChannel  = std::max(0.2f, std::min(1.0f, 0.7f - (SharedState::level * 0.08f) + impactGlow));

        // Procedural structural properties
        const int sectors = 60;
        const int ringCount = 8;
        const float maxRadius = ringCount * 0.1f;

        // A. Render Focal Point Core Hub
        glLineWidth(2.5f);
        glColor3f(0.0f, 1.0f, 1.0f); // High-contrast neon cyan aesthetic
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 30; i++) {
            float a = i * (2.0f * M_PI) / 30;
            glVertex2f(std::cos(a) * 0.05f, std::sin(a) * 0.05f);
        }
        glEnd();

        // Reset pipeline primitive properties for secondary layout passes
        glLineWidth(1.0f);
        glColor3f(redChannel, greenChannel, blueChannel);

        // B. Render Concentric Structural Grid Rings via Polar Coordinates
        for (int r = 1; r <= ringCount; r++) {
            float radius = r * 0.1f;
            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < sectors; i++) {
                float a = i * (2.0f * M_PI) / sectors;
                glVertex2f(std::cos(a) * radius, std::sin(a) * radius);
            }
            glEnd();
        }

        // C. Render Radial Intersecting Grid Spokes
        glBegin(GL_LINES);
        for (int i = 0; i < sectors; i += 5) { // Fixed-angle angular stride (30-degree partitions)
            float a = i * (2.0f * M_PI) / sectors;
            float cosA = std::cos(a);
            float sinA = std::sin(a);

            glVertex2f(0.0f, 0.0f);
            glVertex2f(cosA * maxRadius, sinA * maxRadius);
        }
        glEnd();

        // Restore original projection/modelview transformation state
        glPopMatrix();
    }
}
