#include "member5_boss.h"
#include "game_engine.h"
#include <cmath>

namespace Member5_Boss
{
    float scaleFactor = 1.0f;
    float morphCycle = 0.0f;

    void update()
    {
        morphCycle += 0.05f;
        scaleFactor = 1.0f + 0.1f * sin(morphCycle);
    }

    void draw()
    {
        glPushMatrix();
        glTranslatef(0.0f, 0.6f, 0.0f);
        glScalef(scaleFactor, scaleFactor, 1.0f);

        glColor3f(0.8f, 0.0f, 0.8f);
        glBegin(GL_LINE_LOOP);
        // Math driven geometric morphing deformation loops
        for (int i = 0; i < 4; i++)
        {
            float angleDeg = i * 90.0f * 3.14159f / 180.0f;
            float deformation = 0.15f + 0.03f * cos(morphCycle + i);
            glVertex2f(cos(angleDeg) * deformation, sin(angleDeg) * deformation);
        }
        glEnd();
        glPopMatrix();
    }
}