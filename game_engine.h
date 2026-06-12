#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <GL/glut.h>
#include <vector>

enum GameState { START_MENU, PLAYING, LEVEL_TRANSITION, GAME_OVER, VICTORY };
enum EnemyType { SWARM, TANK };

struct Enemy { float x, y, speed; int hp; EnemyType type; };
struct Projectile { float x, y, dx, dy; bool active; bool isEnemyWeapon; };

namespace SharedState {
    extern GameState gameState;
    extern int winWidth, winHeight;
    extern float mouseGL_X, mouseGL_Y;
    extern int score, highScore, lives, level, wave;
    extern float screenShake;
    extern int hitStopFrames;

    extern std::vector<Enemy> enemies;
    extern std::vector<Projectile> projectiles;

    extern float playerX, playerY, playerAngle;
    extern bool isDashing;
    extern int dashTimer;
    extern float dashDx, dashDy;
    extern int invulnTimer;

    extern bool bossActive;
    extern float bossX, bossY, bossMaxHP, bossHP;
    extern int bossAttackTimer;
}

#endif
