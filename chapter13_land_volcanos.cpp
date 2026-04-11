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
    volcanoes.push_back({-8.0f, -5.0f, 5.5f, 0.7f});
    volcanoes.push_back({0.0f, 0.0f, 6.5f, 0.5f});
    volcanoes.push_back({8.0f, -4.0f, 5.2f, 0.6f});
}

void initializeGymnosperms() {
    gymnosperms.clear();
    // Create a large island formation with HUGE pine trees
    // Center cluster (main island)
    for (int i = 0; i < 25; i++) {
        float angle = (i / 25.0f) * 6.283f;
        float distance = 3.0f + (rand() % 50) / 20.0f;
        float x = 0.0f + cosf(angle) * distance;
        float z = 0.0f + sinf(angle) * distance;
        float h = 3.5f + (rand() % 50) / 100.0f;
        gymnosperms.push_back({x, z, h});
    }
    // Left island cluster
    for (int i = 0; i < 15; i++) {
        float x = -8.0f + (rand() % 40) / 20.0f;
        float z = -5.0f + (rand() % 40) / 20.0f;
        float h = 3.2f + (rand() % 40) / 100.0f;
        gymnosperms.push_back({x, z, h});
    }
    // Right island cluster
    for (int i = 0; i < 15; i++) {
        float x = 8.0f + (rand() % 40) / 20.0f;
        float z = -4.0f + (rand() % 40) / 20.0f;
        float h = 3.0f + (rand() % 40) / 100.0f;
        gymnosperms.push_back({x, z, h});
    }
}

// Draw a gymnosperm (HUGE cone-shaped tree)
void drawGymnosperm(float x, float z, float height) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    
    // Trunk (large cylinder)
    GLUquadric* quad = gluNewQuadric();
    glColor4f(0.5f, 0.3f, 0.1f, 0.95f);
    gluCylinder(quad, 0.35f, 0.25f, height * 0.2f, 12, 12);
    
    // Multiple cone layers for dense foliage
    // Layer 1 - base
    glPushMatrix();
    glTranslatef(0.0f, height * 0.2f, 0.0f);
    glColor4f(0.15f, 0.55f, 0.15f, 0.90f);
    
    float sway = sinf(globalTime * 0.8f + x * 0.05f) * 5.0f;
    glRotatef(sway, 0.0f, 0.0f, 1.0f);
    
    glutSolidCone(height * 0.5f, height * 0.45f, 20, 20);
    glPopMatrix();
    
    // Layer 2 - middle
    glPushMatrix();
    glTranslatef(0.0f, height * 0.45f, 0.0f);
    glColor4f(0.18f, 0.60f, 0.18f, 0.88f);
    float sway2 = sinf(globalTime * 0.7f + x * 0.07f) * 4.0f;
    glRotatef(sway2, 0.0f, 0.0f, 1.0f);
    glutSolidCone(height * 0.35f, height * 0.38f, 20, 20);
    glPopMatrix();
    
    // Layer 3 - top
    glPushMatrix();
    glTranslatef(0.0f, height * 0.70f, 0.0f);
    glColor4f(0.20f, 0.65f, 0.20f, 0.85f);
    float sway3 = sinf(globalTime * 0.9f + x * 0.08f) * 6.0f;
    glRotatef(sway3, 0.0f, 0.0f, 1.0f);
    glutSolidCone(height * 0.20f, height * 0.30f, 18, 18);
    glPopMatrix();
    
    glPopMatrix();
}

// Draw a volcano - HUGE VERTICAL CONE STRUCTURE
void drawVolcano(float x, float z, float height, float time) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    
    // Main cone - HUGE vertical volcano
    glColor4f(0.7f, 0.4f, 0.15f, 1.0f);
    glutSolidCone(height * 1.8f, height * 3.5f, 32, 32);
    
    // Darker layer at base for depth
    glColor4f(0.5f, 0.25f, 0.08f, 0.95f);
    GLUquadric* quadBase = gluNewQuadric();
    gluDisk(quadBase, 0.0f, height * 1.8f, 24, 4);
    
    // Crater rim - circular opening at top
    glPushMatrix();
    glTranslatef(0.0f, height * 3.5f, 0.0f);
    glColor4f(0.1f, 0.05f, 0.0f, 1.0f);
    GLUquadric* quadCrater = gluNewQuadric();
    gluDisk(quadCrater, 0.0f, height * 0.7f, 24, 4);
    glPopMatrix();
    
    // Lava eruption - shooting upward from crater
    float eruptionFactor = sinf(time * 4.0f) * 0.6f + 0.4f;
    for (int i = 0; i < 12; i++) {
        glPushMatrix();
        float angle = (i / 12.0f) * 6.283f;
        float offsetX = cosf(angle) * height * 0.6f;
        float offsetZ = sinf(angle) * height * 0.6f;
        float offsetY = height * 3.5f + eruptionFactor * height * 1.2f;
        glTranslatef(offsetX, offsetY, offsetZ);
        
        glColor4f(1.0f, 0.3f + eruptionFactor * 0.4f, 0.0f, 0.6f * eruptionFactor);
        glutSolidSphere(height * 0.35f * eruptionFactor, 12, 12);
        glPopMatrix();
    }
    
    // Smoke/ash cloud above volcano - larger
    glPushMatrix();
    glTranslatef(0.0f, height * 3.5f + height * 1.2f, 0.0f);
    glColor4f(0.6f, 0.6f, 0.6f, 0.4f * eruptionFactor);
    glutSolidSphere(height * 1.0f, 14, 14);
    glPopMatrix();
    
    // Additional ash clouds for atmosphere
    glPushMatrix();
    glTranslatef(height * 0.8f, height * 3.5f + height * 0.9f, height * 0.5f);
    glColor4f(0.5f, 0.5f, 0.5f, 0.25f * eruptionFactor);
    glutSolidSphere(height * 0.7f, 10, 10);
    glPopMatrix();
    
    glPopMatrix();
}

// Draw ground (island)
void drawGround() {
    // Inner island land
    glColor4f(0.35f, 0.55f, 0.25f, 0.95f);
    glBegin(GL_QUADS);
    glVertex3f(-12.0f, -0.05f, -8.0f);
    glVertex3f(12.0f, -0.05f, -8.0f);
    glVertex3f(12.0f, -0.05f, 8.0f);
    glVertex3f(-12.0f, -0.05f, 8.0f);
    glEnd();
    
    // Water around island
    glColor4f(0.1f, 0.3f, 0.6f, 0.8f);
    glBegin(GL_QUADS);
    glVertex3f(-16.0f, -0.08f, -12.0f);
    glVertex3f(16.0f, -0.08f, -12.0f);
    glVertex3f(16.0f, -0.08f, 12.0f);
    glVertex3f(-16.0f, -0.08f, 12.0f);
    glEnd();
    
    // Ground grid on island
    glColor4f(0.45f, 0.65f, 0.35f, 0.6f);
    glBegin(GL_LINES);
    for (float x = -12.0f; x <= 12.0f; x += 3.0f) {
        glVertex3f(x, 0.0f, -8.0f);
        glVertex3f(x, 0.0f, 8.0f);
    }
    for (float z = -8.0f; z <= 8.0f; z += 3.0f) {
        glVertex3f(-12.0f, 0.0f, z);
        glVertex3f(12.0f, 0.0f, z);
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
    
    // Camera overview of volcanic island with giant pine forest
    gluLookAt(
        5.0f, 8.0f, 16.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );
    
    // Draw environment
    drawGround();
    
    // Draw volcanoes with eruption
    for (const auto& vol : volcanoes) {
        drawVolcano(vol.x, vol.z, vol.height, globalTime * vol.eruptionIntensity);
    }
    
    // Draw gymnosperms (giant pine trees)
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
    const char* title = "Chapter 13: Volcanic Island with Giant Gynosperms";
    for (const char* c = title; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    
    glRasterPos2f(50, 80);
    char timeStr[50];
    sprintf(timeStr, "Time: %.1f seconds | Triple volcanoes erupting", globalTime);
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
