#include "game_engine.h"

namespace SharedState {
    // Game System
    GameState gameState = START_MENU;
    int winWidth = 1200, winHeight = 800;
    float mouseGL_X = 0, mouseGL_Y = 0;

    // Player Stats
    int score = 0, highScore = 0;
    int lives = 5;
    int level = 1;
    int wave = 1;

    // Juice / Game Feel
    float screenShake = 0.0f;
    int hitStopFrames = 0;

    // Entity Vectors
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;

    // Player Physics & State
    float playerX = 0, playerY = 0, playerAngle = 0;
    bool isDashing = false;
    int dashTimer = 0;
    float dashDx = 0, dashDy = 0;
    int invulnTimer = 0;

    // Boss State
    bool bossActive = false;
    float bossX = 0.0f, bossY = 0.6f;
    float bossMaxHP = 150.0f;
    float bossHP = 150.0f;
    int bossAttackTimer = 0;
}
