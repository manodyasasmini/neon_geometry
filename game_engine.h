#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <GL/glut.h>
#include <vector>

// --- ENUMS ---
enum GameState { START_MENU, PLAYING, LEVEL_TRANSITION, GAME_OVER, VICTORY };
enum EnemyType { SWARM, TANK };
enum PowerUpType { HEALTH_REGEN, SPREAD_SHOT, RAPID_FIRE }; // NEW: For power-up drops

// --- STRUCTS ---
struct Enemy {
    float x, y, speed;
    int hp;
    EnemyType type;
    float spawnProgress;
};

struct Projectile {
    float x, y, dx, dy;
    bool active;
    bool isEnemyWeapon;
};

// NEW: Prepares the math for Member 6's Explosion System
struct Particle {
    float x, y, dx, dy;
    float life;       // Fades out as it reaches 0
    float r, g, b;    // Allows for different colored explosions (e.g. Red for tanks, yellow for swarm)
};

// NEW: Prepares the game for tactical drops
struct PowerUp {
    float x, y;
    PowerUpType type;
    int durationTimer; // How long it stays on screen before disappearing
    float pulseScale;  // For breathing/glowing animation
};

// --- SHARED STATE DECLARATIONS ---
namespace SharedState {
    extern GameState gameState;
    extern int winWidth, winHeight;
    extern float mouseGL_X, mouseGL_Y;

    extern int score, highScore, lives, level, wave;
    extern float screenShake;
    extern int hitStopFrames;

    extern std::vector<Enemy> enemies;
    extern std::vector<Projectile> projectiles;
    extern std::vector<PowerUp> powerups; // NEW: Tracks active powerups on the ground

    extern float playerX, playerY, playerAngle;
    extern bool isDashing;
    extern int dashTimer;
    extern float dashDx, dashDy;
    extern int invulnTimer;

    // NEW: Player Weapon States (Triggered by PowerUps)
    extern bool hasSpreadShot;
    extern int weaponTimer;

    extern bool bossActive;
    extern float bossX, bossY, bossMaxHP, bossHP;
    extern int bossAttackTimer;
}

#endif // GAME_ENGINE_H
