#include "member4_grid.h"
#include "game_engine.h"
#include <cmath>
#include <algorithm>

namespace Member4_Grid
{
    float pulseAngle = 0.0f;
    float scaleMod = 1.0f;
    float shearAmount = 0.0f;

    void update()
    {
        // Compute scale factor for the pulsing breathing effect
        pulseAngle += 0.03f;
        scaleMod = 1.0f + 0.04f * std::sin(pulseAngle);

        // Linearly ramp shear intensity up during dash, decay down when walking
        if (SharedState::isDashing)
        {
            if (shearAmount < 1.5f)
                shearAmount += 0.1f;
        }
        else
        {
            if (shearAmount > 0.0f)
                shearAmount -= 0.05f;
        }
    }

    // Midpoint Circle Drawing Algorithm using 8-way symmetry
    void drawMidpointCircle(float radius)
    {
        glBegin(GL_POINTS);

        // Scale radius up to a high-density integer grid to eliminate pixel gaps
        float pixelGridScale = 2500.0f;
        int r = static_cast<int>(radius * pixelGridScale);
        if (r <= 0)
            return;

        int x = 0;
        int y = r;
        int d = 1 - r; // Initial decision parameter

        while (x <= y)
        {
            // Map integer coordinates back to OpenGL floating-point viewport space
            float fx = x / pixelGridScale;
            float fy = y / pixelGridScale;

            // Plot calculated point across all 8 symmetrical octants
            glVertex2f(fx, fy);
            glVertex2f(fy, fx);
            glVertex2f(-fx, fy);
            glVertex2f(-fy, fx);
            glVertex2f(-fx, -fy);
            glVertex2f(-fy, -fx);
            glVertex2f(fx, -fy);
            glVertex2f(fy, -fx);

            x++;
            if (d < 0)
            {
                d += 2 * x + 3;
            }
            else
            {
                y--;
                d += 2 * (x - y) + 5;
            }
        }
        glEnd();
    }

    // Digital Differential Analyzer (DDA) Line Drawing Algorithm
    void drawDDALine(float x1, float y1, float x2, float y2)
    {
        float pX1 = x1 * 600.0f;
        float pY1 = y1 * 600.0f;
        float pX2 = x2 * 600.0f;
        float pY2 = y2 * 600.0f;

        float dx = pX2 - pX1;
        float dy = pY2 - pY1;

        int steps = std::max(std::abs(dx), std::abs(dy));
        if (steps == 0)
            return;

        float xIncrement = dx / steps;
        float yIncrement = dy / steps;

        float currentX = pX1;
        float currentY = pY1;

        glBegin(GL_POINTS);
        for (int i = 0; i <= steps; i++)
        {
            glVertex2f(currentX / 600.0f, currentY / 600.0f);
            currentX += xIncrement;
            currentY += yIncrement;
        }
        glEnd();
    }

    void draw()
    {
        glPushMatrix();

        // 1. Geometric Transformation: Parallax Translation
        float parallaxX = -SharedState::playerX * 0.12f;
        float parallaxY = -SharedState::playerY * 0.12f;
        glTranslatef(parallaxX, parallaxY, 0.0f);

        // 2. Geometric Transformation: Custom Column-Major Horizontal Shear
        float dynamicShear = 0.0f;
        if (SharedState::isDashing)
        {
            dynamicShear = SharedState::dashDx * shearAmount * 2.0f;
        }

        float simpleShearMatrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            dynamicShear, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};
        glMultMatrixf(simpleShearMatrix);

        // 3. Geometric Transformation: Scaling
        glScalef(scaleMod, scaleMod, 1.0f);

        // Structural configurations for raster grid primitives
        const int ringCount = 8;
        const float ringSpacing = 0.1f;
        glPointSize(1.5f);

        // Draw Focal Core Hub
        glColor3f(0.0f, 1.0f, 1.0f);
        drawMidpointCircle(0.05f);

        // Draw Concentric Structural Grid Rings
        glColor3f(0.1f, 0.3f, 0.6f);
        for (int r = 1; r <= ringCount; r++)
        {
            drawMidpointCircle(r * ringSpacing);
        }

        // Draw Radial Intersecting Grid Spokes
        float maxRadius = ringCount * ringSpacing;
        float targetAngles[12] = {0.0f, 0.52f, 1.04f, 1.57f, 2.09f, 2.61f, 3.14f, 3.66f, 4.18f, 4.71f, 5.23f, 5.75f};

        for (int i = 0; i < 12; i++)
        {
            float endX = std::cos(targetAngles[i]) * maxRadius;
            float endY = std::sin(targetAngles[i]) * maxRadius;
            drawDDALine(0.0f, 0.0f, endX, endY);
        }

        glPopMatrix();
    }
}