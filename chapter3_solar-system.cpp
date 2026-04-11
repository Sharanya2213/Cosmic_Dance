#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 900;
const float ANIMATION_DURATION = 12.0f;

struct Vec2 {
    float x, y;
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
};

struct Particle {
    Vec2 pos;
    Vec2 vel;
    float size;
    float life;
};

vector<Particle> fluidParticles;
vector<Vec2> cellVertices;
vector<Vec2> nucleusVertices;
vector<Vec2> organelles;

float globalTime = 0.0f;
float cellX = 0.0f;
float cellY = 0.0f;

const float BASE_CELL_RADIUS = 1.2f;
const float NUCLEUS_RADIUS = 0.35f;
const int NUM_CELL_VERTICES = 40;
const int NUM_NUCLEUS_VERTICES = 25;

void generateFluidParticles() {
    fluidParticles.clear();
    for (int i = 0; i < 800; i++) {
        Particle p;
        p.pos = Vec2((rand() % 1000 - 500) * 0.01f, (rand() % 1000 - 500) * 0.01f);
        p.vel = Vec2((rand() % 100 - 50) * 0.0001f, (rand() % 100 - 50) * 0.0001f);
        p.size = 0.02f + (rand() % 20) * 0.001f;
        p.life = 1.0f;
        fluidParticles.push_back(p);
    }
}

void generateOrganelles() {
    organelles.clear();
    organelles.push_back(Vec2(-0.15f, 0.1f));
    organelles.push_back(Vec2(0.2f, -0.05f));
    organelles.push_back(Vec2(-0.05f, -0.15f));
    organelles.push_back(Vec2(0.1f, 0.15f));
    organelles.push_back(Vec2(0.0f, 0.0f));
}

void generateCellVertices() {
    cellVertices.clear();
    for (int i = 0; i < NUM_CELL_VERTICES; i++) {
        float angle = (i / (float)NUM_CELL_VERTICES) * 2.0f * 3.14159f;
        float distortion = 0.9f + 0.15f * sin(angle * 3 + globalTime * 1.5f) + 0.1f * sin(angle * 7 - globalTime * 0.8f) + 0.08f * cos(angle * 2 + globalTime * 0.5f);
        float radius = BASE_CELL_RADIUS * distortion;
        float pulse = 1.0f + sin(globalTime * 2.0f) * 0.05f;
        radius *= pulse;
        Vec2 vertex;
        vertex.x = cellX + cos(angle) * radius;
        vertex.y = cellY + sin(angle) * radius;
        cellVertices.push_back(vertex);
    }
}

void generateNucleusVertices() {
    nucleusVertices.clear();
    for (int i = 0; i < NUM_NUCLEUS_VERTICES; i++) {
        float angle = (i / (float)NUM_NUCLEUS_VERTICES) * 2.0f * 3.14159f;
        float distortion = 0.85f + 0.1f * sin(angle * 4 + globalTime * 1.2f) + 0.08f * sin(angle * 2 - globalTime * 0.6f);
        float radius = NUCLEUS_RADIUS * distortion;
        Vec2 vertex;
        vertex.x = cellX + cos(angle) * radius;
        vertex.y = cellY + sin(angle) * radius;
        nucleusVertices.push_back(vertex);
    }
}

void updateFluidParticles(float dt) {
    for (auto& p : fluidParticles) {
        p.pos.x += p.vel.x * dt;
        p.pos.y += p.vel.y * dt;
        p.vel.x += (rand() % 100 - 50) * 0.00001f;
        p.vel.y += (rand() % 100 - 50) * 0.00001f;
        p.vel.x *= 0.99f;
        p.vel.y *= 0.99f;
        if (p.pos.x > 5.0f) p.pos.x = -5.0f;
        if (p.pos.x < -5.0f) p.pos.x = 5.0f;
        if (p.pos.y > 5.0f) p.pos.y = -5.0f;
        if (p.pos.y < -5.0f) p.pos.y = 5.0f;
    }
}

void updateCellPosition() {
    cellX = 0.2f * sin(globalTime * 0.3f);
    cellY = 0.15f * cos(globalTime * 0.25f);
}

void drawGradientBackground() {
    glDisable(GL_BLEND);
    glBegin(GL_QUADS);
    glColor3f(0.1f, 0.25f, 0.2f);
    glVertex2f(-5.0f, 5.0f);
    glVertex2f(5.0f, 5.0f);
    glColor3f(0.15f, 0.3f, 0.25f);
    glVertex2f(5.0f, -5.0f);
    glVertex2f(-5.0f, -5.0f);
    glEnd();
}

void drawFluidParticles() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (const auto& p : fluidParticles) {
        glColor4f(0.6f, 0.7f, 0.5f, 0.3f);
        glVertex2f(p.pos.x, p.pos.y);
    }
    glEnd();
    glPointSize(1.0f);
    glDisable(GL_BLEND);
}

void drawCell() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.2f, 0.7f, 0.5f, 0.75f);
    glBegin(GL_POLYGON);
    for (const auto& v : cellVertices) {
        glVertex2f(v.x, v.y);
    }
    glEnd();
    glColor3f(0.15f, 0.5f, 0.35f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    for (const auto& v : cellVertices) {
        glVertex2f(v.x, v.y);
    }
    glEnd();
    glLineWidth(1.0f);
    glColor4f(0.9f, 0.85f, 0.2f, 0.8f);
    glBegin(GL_POLYGON);
    for (const auto& v : nucleusVertices) {
        glVertex2f(v.x, v.y);
    }
    glEnd();
    glColor3f(0.7f, 0.65f, 0.1f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    for (const auto& v : nucleusVertices) {
        glVertex2f(v.x, v.y);
    }
    glEnd();
    glPointSize(4.0f);
    glColor4f(1.0f, 0.5f, 0.2f, 0.8f);
    glBegin(GL_POINTS);
    for (const auto& org : organelles) {
        glVertex2f(cellX + org.x, cellY + org.y);
    }
    glEnd();
    glPointSize(1.0f);
    glDisable(GL_BLEND);
}

void drawNucleolus() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(3.0f);
    glColor4f(0.95f, 0.9f, 0.3f, 0.9f);
    glBegin(GL_POINTS);
    glVertex2f(cellX, cellY);
    glEnd();
    glPointSize(1.0f);
    glDisable(GL_BLEND);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-5.0f, 5.0f, -5.0f, 5.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawGradientBackground();
    drawFluidParticles();
    drawCell();
    drawNucleolus();
    glutSwapBuffers();
}

void timer(int value) {
    float dt = 0.016f;
    globalTime += dt;
    if (globalTime > ANIMATION_DURATION) {
        globalTime = 0.0f;
    }
    updateFluidParticles(dt);
    updateCellPosition();
    generateCellVertices();
    generateNucleusVertices();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void init() {
    glClearColor(0.15f, 0.3f, 0.25f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    generateFluidParticles();
    generateOrganelles();
    generateCellVertices();
    generateNucleusVertices();
    printf("=== FIRST LIFE ON EARTH ==================\n");
    printf("Microscopic view of a primitive cell\n");
    printf("In ancient Earth's primordial oceans\n");
    printf("Observe organic motion and life\n");
    printf("Controls: ESC to exit | 12 sec loop\n");
    printf("======================================\n");
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("First Life on Earth - Microscopic View");
    init();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-5.0f, 5.0f, -5.0f, 5.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);
    glutMainLoop();
    return 0;
}
