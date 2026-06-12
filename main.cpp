#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Link all modular system interfaces
#include "game_engine.h"
#include "member1_player.h"
#include "member2_swarm.h"
#include "member3_lasers.h"
#include "member4_grid.h"
#include "member5_boss.h"
#include "member6_particles.h"

// --- ENGINE RECONCILIATION ---
void checkCollisions() {
    // Inter-module collision bridge logic
    for (auto& l : Member3_Lasers::pool) {
        if (!l.active) continue;
        for (auto& e : Member2_Swarm::pool) {
            if (!e.active) continue;

            float dist = sqrt(pow(l.x - e.x, 2) + pow(l.y - e.y, 2));
            if (dist < 0.06f) {
                l.active = false;
                e.active = false;
                Member6_Particles::triggerExplosion(e.x, e.y);
            }
        }
    }
}

// --- OPENGL CALLBACK ROUTINES ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Sequentially render elements in structural visual depth order
    Member4_Grid::draw();
    Member5_Boss::draw();
    Member2_Swarm::draw();
    Member3_Lasers::draw();
    Member6_Particles::draw();
    Member1_Player::draw();

    glutSwapBuffers();
}

void update(int value) {
    // Fire off updates across individual developer sandboxes
    Member1_Player::update();
    Member4_Grid::update();
    Member5_Boss::update();
    Member2_Swarm::update();
    Member3_Lasers::update();
    Member6_Particles::update();

    checkCollisions();

    // Clean up entity lists safely out of loops
    Member2_Swarm::pool.erase(std::remove_if(Member2_Swarm::pool.begin(), Member2_Swarm::pool.end(),
        [](const Member2_Swarm::Enemy& e){ return !e.active; }), Member2_Swarm::pool.end());

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.05f;
    if (key == 'w' || key == 'W') Member1_Player::y += moveSpeed;
    if (key == 's' || key == 'S') Member1_Player::y -= moveSpeed;
    if (key == 'a' || key == 'A') Member1_Player::x -= moveSpeed;
    if (key == 'd' || key == 'D') Member1_Player::x += moveSpeed;
}

void mousePassive(int x, int y) {
    SharedState::mouseX = ((float)x / SharedState::winWidth) * 2.0f - 1.0f;
    SharedState::mouseY = 1.0f - ((float)y / SharedState::winHeight) * 2.0f;
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Member3_Lasers::fire();
    }
}

void reshape(int w, int h) {
    SharedState::winWidth = w;
    SharedState::winHeight = h;
    glViewport(0, 0, w, h);
}

// --- ENGINE ENTRY POINT ---
int main(int argc, char** argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SharedState::winWidth, SharedState::winHeight);
    glutCreateWindow("Neon Geometry: Component Assembly Build");

    glClearColor(0.01f, 0.01f, 0.03f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mousePassive);
    glutMouseFunc(mouseClick);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
