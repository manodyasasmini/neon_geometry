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


    }

