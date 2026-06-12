#include "game_engine.h"

namespace SharedState {
    GameState gameState = START_MENU;
    int winWidth = 1200, winHeight = 800;
    float mouseGL_X = 0, mouseGL_Y = 0;
    int score = 0, highScore = 0;
    int lives = 5;
    int level = 1;
    int wave = 1;
    float screenShake = 0.0f;
    int hitStopFrames = 0;

    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;

    float playerX = 0, playerY = 0, playerAngle = 0;
    bool isDashing = false;
    int dashTimer = 0;
    float dashDx = 0, dashDy = 0;
    int invulnTimer = 0;

    bool bossActive = false;
    float bossX = 0.0f, bossY = 0.6f;
    float bossMaxHP = 150.0f;
    float bossHP = 150.0f;
    int bossAttackTimer = 0;
}
