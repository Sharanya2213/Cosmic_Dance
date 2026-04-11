#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>

float globalTime = 0.0f;

// Volcano structure
struct Volcano {
    float x, z;
    float height;
    float eruptionIntensity;
};

struct Gymnosperm {
    float x, z;
    float height;
};

std::vector<Volcano> volcanoes;
std::vector<Gymnosperm> gymnosperms;

void initializeVolcanoes() {
    volcanoes.clear();
    volcanoes.push_back({-5.0f, -3.0f, 2.5f, 0.7f});
    volcanoes.push_back({3.0f, 4.0f, 3.0f, 0.5f});
    volcanoes.push_back({6.0f, -2.0f, 2.2f, 0.6f});
}

void initializeGymnosperms() {
    gymnosperms.clear();
    for (int i = 0; i < 12; i++) {
        float x = -4.0f + (rand() % 120) / 10.0f;
        float z = -3.0f + (rand() % 60) / 10.0f;
        float h = 0.8f + (rand() % 30) / 100.0f;
        gymnosperms.push_back({x, z, h});
    }
}

// Draw a gymnosperm (cone-shaped tree)
void drawGymnosperm(float x, float z, float height) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    
    // Trunk (cylinder)
    GLUquadric* quad = gluNewQuadric();
    glColor4f(0.4f, 0.25f, 0.1f, 0.9f);
    gluCylinder(quad, 0.08f, 0.06f, height * 0.3f, 8, 8);
    
    // Foliage (cone)
    glTranslatef(0.0f, height * 0.3f, 0.0f);
    glColor4f(0.2f, 0.6f, 0.2f, 0.85f);
    
    // Slight sway
    float sway = sinf(globalTime * 1.2f + x * 0.1f) * 3.0f;
    glRotatef(sway, 0.0f, 0.0f, 1.0f);
    
    glutSolidCone(height * 0.25f, height * 0.7f, 16, 16);
    
    glPopMatrix();
}

// Draw a volcano - VERTICAL CONE STRUCTURE
void drawVolcano(float x, float z, float height, float time) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    
    // Main cone - large vertical volcano
    glColor4f(0.6f, 0.35f, 0.15f, 1.0f);
    glutSolidCone(height * 1.2f, height * 2.5f, 24, 24);
    
    // Crater rim - circular opening at top
    glPushMatrix();
    glTranslatef(0.0f, height * 2.5f, 0.0f);
    glColor4f(0.2f, 0.1f, 0.0f, 1.0f);
    GLUquadric* quadCrater = gluNewQuadric();
    gluDisk(quadCrater, 0.0f, height * 0.5f, 20, 4);
    glPopMatrix();
    
    // Lava eruption - shooting upward from crater
    float eruptionFactor = sinf(time * 4.0f) * 0.6f + 0.4f;
    for (int i = 0; i < 8; i++) {
        glPushMatrix();
        float angle = (i / 8.0f) * 6.283f;
        float offsetX = cosf(angle) * height * 0.4f;
        float offsetZ = sinf(angle) * height * 0.4f;
        float offsetY = height * 2.5f + eruptionFactor * height;
        glTranslatef(offsetX, offsetY, offsetZ);
        
        glColor4f(1.0f, 0.5f + eruptionFactor * 0.3f, 0.0f, 0.5f * eruptionFactor);
        glutSolidSphere(height * 0.25f * eruptionFactor, 10, 10);
        glPopMatrix();
    }
    
    // Smoke/ash cloud above volcano
    glPushMatrix();
    glTranslatef(0.0f, height * 2.5f + height * 0.8f, 0.0f);
    glColor4f(0.5f, 0.5f, 0.5f, 0.3f * eruptionFactor);
    glutSolidSphere(height * 0.6f, 12, 12);
    glPopMatrix();
    
    glPopMatrix();
}

// Draw ground
void drawGround() {
    glColor4f(0.3f, 0.5f, 0.2f, 0.95f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, -0.05f, -5.0f);
    glVertex3f(10.0f, -0.05f, -5.0f);
    glVertex3f(10.0f, -0.05f, 5.0f);
    glVertex3f(-10.0f, -0.05f, 5.0f);
    glEnd();
    
    // Ground grid
    glColor4f(0.4f, 0.6f, 0.3f, 0.6f);
    glBegin(GL_LINES);
    for (float x = -10.0f; x <= 10.0f; x += 2.0f) {
        glVertex3f(x, 0.0f, -5.0f);
        glVertex3f(x, 0.0f, 5.0f);
    }
    for (float z = -5.0f; z <= 5.0f; z += 2.0f) {
        glVertex3f(-10.0f, 0.0f, z);
        glVertex3f(10.0f, 0.0f, z);
    }
    glEnd();
}

// Draw sky
void drawSky() {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    // Sky gradient
    glBegin(GL_QUADS);
    // Light blue at horizon
    glColor4f(0.5f, 0.7f, 1.0f, 1.0f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    // Darker blue at top
    glColor4f(0.2f, 0.4f, 0.8f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
    
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera overview of land with volcanoes
    gluLookAt(
        2.0f, 4.0f, 10.0f,
        2.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );
    
    // Draw environment
    drawGround();
    
    // Draw volcanoes with eruption
    for (const auto& vol : volcanoes) {
        drawVolcano(vol.x, vol.z, vol.height, globalTime * vol.eruptionIntensity);
    }
    
    // Draw gymnosperms
    for (const auto& tree : gymnosperms) {
        drawGymnosperm(tree.x, tree.z, tree.height);
    }
    
    // Draw text overlay
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glColor4f(0.8f, 0.9f, 1.0f, 0.95f);
    glRasterPos2f(50, 50);
    const char* title = "Chapter 13: Land with Volcanoes & Gymnosperms";
    for (const char* c = title; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    
    glRasterPos2f(50, 80);
    char timeStr[50];
    sprintf(timeStr, "Time: %.1f seconds | Volcanoes erupting", globalTime);
    for (const char* c = timeStr; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    globalTime += 0.016f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_position[] = { 5.0f, 10.0f, 5.0f, 0.0f };
    GLfloat light_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat material_shininess[] = { 32.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
}

void init() {
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    setupLighting();
    initializeVolcanoes();
    initializeGymnosperms();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Chapter 13: Land with Volcanoes & Gymnosperms");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}
