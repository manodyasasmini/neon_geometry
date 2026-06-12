#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <GL/glut.h>

// Shared environment states accessible across individual files
namespace SharedState
{
    extern int winWidth;
    extern int winHeight;
    extern float mouseX;
    extern float mouseY;

    struct PlayerData
    {
        float x, y, angle;
    };
    extern PlayerData player;
}

#endif