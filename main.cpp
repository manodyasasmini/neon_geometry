#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "game_engine.h"
#include "member1_player.h"
#include "member2_swarm.h"
#include "member3_lasers.h"
#include "member4_grid.h"
#include "member5_boss.h"
#include "member6_particles.h"

int bossWaveTimer = 0;
bool keyState[256] = { false };
bool specialKeyState[256] = { false };

void drawText(float x, float y, const std::string& s) {
    glRasterPos2f(x, y);
    for (char c : s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void drawGlassRect(float x1, float y1, float x2, float y2) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.05f, 0.05f, 0.1f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1); glVertex2f(x2, y1);
    glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
    glDisable(GL_BLEND);
}

void applyScreenShake() {
    if (SharedState::screenShake > 0.0f) {
        float dx = ((rand() % 100) / 100.0f - 0.5f) * SharedState::screenShake;
        float dy = ((rand() % 100) / 100.0f - 0.5f) * SharedState::screenShake;
        glTranslatef(dx, dy, 0);
        SharedState::screenShake -= 0.005f;
    }
}

void startNextLevel() {
    SharedState::level++; SharedState::wave = 1;
    SharedState::bossActive = (SharedState::level >= 3);
    SharedState::bossMaxHP = 150.0f + (SharedState::level * 50.0f);
    SharedState::bossHP = SharedState::bossMaxHP;
    SharedState::playerX = 0; SharedState::playerY = 0;
    SharedState::projectiles.clear(); SharedState::enemies.clear();
    SharedState::gameState = PLAYING; bossWaveTimer = 0;
    std::fill(std::begin(keyState), std::end(keyState), false);
    std::fill(std::begin(specialKeyState), std::end(specialKeyState), false);
    Member2_Swarm::spawnWave();
}

void restartGame() {
    SharedState::score = 0; SharedState::lives = 5; SharedState::level = 1; SharedState::wave = 1;
    SharedState::bossActive = (SharedState::level >= 3); SharedState::bossMaxHP = 150.0f; SharedState::bossHP = 150.0f;
    SharedState::playerX = 0; SharedState::playerY = 0;
    SharedState::projectiles.clear(); SharedState::enemies.clear();
    SharedState::gameState = PLAYING; bossWaveTimer = 0;
    std::fill(std::begin(keyState), std::end(keyState), false);
    std::fill(std::begin(specialKeyState), std::end(specialKeyState), false);
    Member2_Swarm::spawnWave();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    applyScreenShake();

    if (SharedState::gameState == START_MENU) {
        Member4_Grid::draw();
        drawGlassRect(-0.4f, -0.2f, 0.4f, 0.3f);
        glColor3f(0, 1, 1); drawText(-0.35f, 0.1f, "NEON GEOMETRY: PROTOCOL");
        glColor3f(1, 1, 1); drawText(-0.25f, -0.05f, "Press [SPACE] to Initialize");
        glutSwapBuffers(); glPopMatrix(); return;
    }

    if (SharedState::gameState == LEVEL_TRANSITION) {
        drawGlassRect(-0.3f, -0.1f, 0.3f, 0.1f);
        glColor3f(0, 1, 0); drawText(-0.15f, 0.0f, "LEVEL CLEARED");
        glutSwapBuffers(); glPopMatrix(); return;
    }

    if (SharedState::gameState == GAME_OVER || SharedState::gameState == VICTORY) {
        drawGlassRect(-0.4f, -0.3f, 0.4f, 0.3f);
        if (SharedState::gameState == VICTORY) { glColor3f(0, 1, 0); drawText(-0.1f, 0.15f, "SYSTEM SECURED - VICTORY"); }
        else { glColor3f(1, 0, 0); drawText(-0.15f, 0.15f, "BREACH DETECTED - GAME OVER"); }
        glColor3f(1, 1, 1);
        drawText(-0.15f, 0.0f, "Final Score: " + std::to_string(SharedState::score));
        drawText(-0.2f, -0.15f, "Press R to Restart | ESC to Quit");
        glutSwapBuffers(); glPopMatrix(); return;
    }

    Member4_Grid::draw();
    Member3_Lasers::draw();
    Member2_Swarm::draw();
    if (SharedState::bossActive) {
        Member5_Boss::draw();
        drawGlassRect(-0.5f, 0.90f, 0.5f, 0.98f);
        glColor3f(1.0f, 0.0f, 0.2f);
        glRectf(-0.48f, 0.92f, -0.48f + (SharedState::bossHP / SharedState::bossMaxHP) * 0.96f, 0.96f);
    }
    Member6_Particles::draw();
    Member1_Player::draw();

    // HUD
    drawGlassRect(-1.0f, 0.85f, -0.6f, 0.98f);
    drawGlassRect(-1.0f, 0.72f, -0.6f, 0.83f);
    drawGlassRect(0.6f, 0.85f, 1.0f, 0.98f);
    glColor3f(1, 1, 1);
    drawText(-0.95f, 0.9f, "Score: " + std::to_string(SharedState::score));
    drawText(-0.95f, 0.76f, "Lvl: " + std::to_string(SharedState::level) + " | Wave: " + (SharedState::bossActive ? "BOSS" : std::to_string(SharedState::wave)));

    for (int i = 0; i < SharedState::lives; i++) {
        glPushMatrix(); glTranslatef(0.65f + (i * 0.06f), 0.92f, 0);
        glColor3f(0, 1, 1); glBegin(GL_TRIANGLES);
        glVertex2f(0, 0.02f); glVertex2f(-0.015f, -0.015f); glVertex2f(0.015f, -0.015f);
        glEnd(); glPopMatrix();
    }
    glPopMatrix(); glutSwapBuffers();
}

void update(int) {
    if (SharedState::hitStopFrames > 0) {
        SharedState::hitStopFrames--; glutPostRedisplay();
        glutTimerFunc(16, update, 0); return;
    }

    if (SharedState::gameState == LEVEL_TRANSITION) {
        if (SharedState::invulnTimer++ > 120) startNextLevel();
    }
    else if (SharedState::gameState == PLAYING) {
        if (SharedState::invulnTimer > 0) SharedState::invulnTimer--;

        // Smooth continuous movement using registered key states
        float moveSpeed = 0.015f;
        if (keyState['w'] || specialKeyState[GLUT_KEY_UP]) SharedState::playerY += moveSpeed;
        if (keyState['s'] || specialKeyState[GLUT_KEY_DOWN]) SharedState::playerY -= moveSpeed;
        if (keyState['a'] || specialKeyState[GLUT_KEY_LEFT]) SharedState::playerX -= moveSpeed;
        if (keyState['d'] || specialKeyState[GLUT_KEY_RIGHT]) SharedState::playerX += moveSpeed;

        if (SharedState::isDashing) {
            SharedState::playerX += SharedState::dashDx; SharedState::playerY += SharedState::dashDy;
            if (--SharedState::dashTimer <= 0) SharedState::isDashing = false;
        }

        SharedState::playerX = std::max(-0.95f, std::min(0.95f, SharedState::playerX));
        SharedState::playerY = std::max(-0.95f, std::min(0.95f, SharedState::playerY));

        Member1_Player::update();
        Member4_Grid::update();
        Member5_Boss::update();
        Member2_Swarm::update();
        Member3_Lasers::update();
        Member6_Particles::update();

        // Collision Checks
        if (SharedState::bossActive && SharedState::invulnTimer == 0) {
            if (sqrt(pow(SharedState::playerX - SharedState::bossX, 2) + pow(SharedState::playerY - SharedState::bossY, 2)) < 0.2f) {
                SharedState::lives--; SharedState::invulnTimer = 60; SharedState::screenShake = 0.08f; SharedState::hitStopFrames = 5;
            }
        }

        for (auto& e : SharedState::enemies) {
            if (e.x > 2) continue;
            float hitBox = (e.type == TANK) ? 0.08f : 0.05f;
            if (SharedState::invulnTimer == 0 && sqrt(pow(SharedState::playerX - e.x, 2) + pow(SharedState::playerY - e.y, 2)) < hitBox) {
                SharedState::lives--; e.x = 5; SharedState::invulnTimer = 60;
                SharedState::screenShake = 0.05f; SharedState::hitStopFrames = 5;
            }
        }

        for (auto& p : SharedState::projectiles) {
            if (!p.active) continue;
            if (p.isEnemyWeapon) {
                if (SharedState::invulnTimer == 0 && sqrt(pow(p.x - SharedState::playerX, 2) + pow(p.y - SharedState::playerY, 2)) < 0.04f) {
                    SharedState::lives--; SharedState::invulnTimer = 60; p.active = false; SharedState::screenShake = 0.05f; SharedState::hitStopFrames = 3;
                }
            } else {
                for (auto& e : SharedState::enemies) {
                    if (e.x > 2) continue;
                    float hitBox = (e.type == TANK) ? 0.08f : 0.05f;
                    if (sqrt(pow(p.x - e.x, 2) + pow(p.y - e.y, 2)) < hitBox) {
                        p.active = false; e.hp--;
                        if (e.hp <= 0) { e.x = 5; SharedState::score += (e.type == TANK ? 30 : 10); Member6_Particles::trigger(p.x, p.y); }
                        break;
                    }
                }
                if (SharedState::bossActive && p.active && sqrt(pow(p.x - SharedState::bossX, 2) + pow(p.y - SharedState::bossY, 2)) < 0.2f) {
                    SharedState::bossHP -= 5.0f; p.active = false; SharedState::score += 20;
                    SharedState::screenShake = 0.02f;
                    if (SharedState::bossHP <= 0) {
                        SharedState::score += 1000 * SharedState::level; SharedState::screenShake = 0.15f; SharedState::hitStopFrames = 20;
                        if (SharedState::level == 3) {
                            if (SharedState::score > SharedState::highScore) SharedState::highScore = SharedState::score;
                            SharedState::gameState = VICTORY;
                        } else { SharedState::gameState = LEVEL_TRANSITION; SharedState::invulnTimer = 0; }
                    }
                }
            }
        }

        SharedState::projectiles.erase(std::remove_if(SharedState::projectiles.begin(), SharedState::projectiles.end(), [](const Projectile& p) { return !p.active; }), SharedState::projectiles.end());

        if (SharedState::lives <= 0) {
            if (SharedState::score > SharedState::highScore) SharedState::highScore = SharedState::score;
            SharedState::gameState = GAME_OVER;
        }

        bool cleared = true;
        for (auto& e : SharedState::enemies) if (e.x < 2) cleared = false;
        
        bossWaveTimer++;
        if (SharedState::level >= 3) {
            SharedState::bossActive = true;
            if (SharedState::wave == 3 && SharedState::bossHP > 0) {
                cleared = false; // Wave 3 doesn't clear until boss dies
            }
        } else {
            SharedState::bossActive = false;
        }

        if (cleared && SharedState::gameState == PLAYING) {
            if (SharedState::level < 3 && SharedState::wave == 3) {
                SharedState::gameState = LEVEL_TRANSITION;
                SharedState::invulnTimer = 0;
            } else {
                SharedState::wave++;
                bossWaveTimer = 0; // Reset timer for next wave
                if (SharedState::wave <= 3) Member2_Swarm::spawnWave();
            }
        }
    }
    glutPostRedisplay(); glutTimerFunc(16, update, 0);
}

void passiveMouse(int x, int y) {
    SharedState::mouseGL_X = ((float)x / SharedState::winWidth) * 2.0f - 1.0f;
    SharedState::mouseGL_Y = 1.0f - ((float)y / SharedState::winHeight) * 2.0f;
}
void activeMouse(int x, int y) { passiveMouse(x, y); }

void mouseClick(int button, int state, int x, int y) {
    if (SharedState::gameState == PLAYING && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Member3_Lasers::fire();
    }
}

void keyboard(unsigned char key, int, int) {
    if (SharedState::gameState == START_MENU && key == ' ') { restartGame(); return; }
    if (SharedState::gameState == GAME_OVER || SharedState::gameState == VICTORY) {
        if (key == 'r' || key == 'R') restartGame();
        if (key == 27) exit(0); return;
    }
    
    unsigned char lowerKey = (key >= 'A' && key <= 'Z') ? (key - 'A' + 'a') : key;
    keyState[lowerKey] = true;

    if (key == ' ' && !SharedState::isDashing && SharedState::gameState == PLAYING) {
        SharedState::isDashing = true; SharedState::dashTimer = 10;
        float dx = SharedState::mouseGL_X - SharedState::playerX, dy = SharedState::mouseGL_Y - SharedState::playerY;
        float len = sqrt(dx*dx + dy*dy);
        if (len > 0) { SharedState::dashDx = (dx/len) * 0.08f; SharedState::dashDy = (dy/len) * 0.08f; }
    }
}

void keyboardUp(unsigned char key, int, int) {
    unsigned char lowerKey = (key >= 'A' && key <= 'Z') ? (key - 'A' + 'a') : key;
    keyState[lowerKey] = false;
}

void specialKeys(int key, int, int) {
    if (key >= 0 && key < 256) {
        specialKeyState[key] = true;
    }
}

void specialKeysUp(int key, int, int) {
    if (key >= 0 && key < 256) {
        specialKeyState[key] = false;
    }
}

void reshape(int w, int h) { SharedState::winWidth = w; SharedState::winHeight = h; glViewport(0, 0, w, h); }

int main(int argc, char** argv) {
    srand(time(0)); glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SharedState::winWidth, SharedState::winHeight);
    glutCreateWindow("Neon Geometry - Master Build");
    glClearColor(0.02f, 0.02f, 0.05f, 1);
    glutDisplayFunc(display); glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard); glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys); glutSpecialUpFunc(specialKeysUp);
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(passiveMouse); glutMotionFunc(activeMouse);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
}
