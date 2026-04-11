#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Global variables
float globalTime = 0.0f;
float fishX = 0.0f;
float fishY = 0.0f;
float fishZ = 0.0f;
float fishSpeed = 1.5f;

// Particle system for underwater effect
struct Particle {
    float x, y, z;
    float vx, vy, vz;
};

std::vector<Particle> particles;

// Initialize particles
void initializeParticles() {
    particles.clear();
    for (int i = 0; i < 300; i++) {
        Particle p;
        p.x = (rand() % 200 - 100) / 10.0f;
        p.y = (rand() % 200 - 100) / 10.0f;
        p.z = (rand() % 100 - 50) / 10.0f;
        p.vx = (rand() % 100 - 50) / 1000.0f;
        p.vy = (rand() % 100 - 50) / 1000.0f;
        p.vz = 0.0f;
        particles.push_back(p);
    }
}

// Draw particles
void drawParticles() {
    glDisable(GL_LIGHTING);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glColor4f(0.6f, 0.8f, 0.9f, 0.3f);
    
    for (auto& p : particles) {
        // Wrap around
        if (p.x > 20.0f + fishX) p.x = -20.0f + fishX;
        if (p.x < -20.0f + fishX) p.x = 20.0f + fishX;
        if (p.y > 15.0f) p.y = -15.0f;
        if (p.y < -15.0f) p.y = 15.0f;
        if (p.z > 5.0f) p.z = -5.0f;
        if (p.z < -5.0f) p.z = 5.0f;
        
        p.x += p.vx;
        p.y += p.vy;
        
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    glPointSize(1.0f);
    glEnable(GL_LIGHTING);
}

// Draw underwater background
void drawUnderwater() {
    glDisable(GL_LIGHTING);
    
    // Draw grid floor
    glColor4f(0.2f, 0.3f, 0.25f, 0.5f);
    glBegin(GL_LINES);
    for (int i = -20; i <= 20; i += 2) {
        glVertex3f(i, -8.0f, -5.0f);
        glVertex3f(i, -8.0f, 5.0f);
        glVertex3f(-20.0f, -8.0f, i);
        glVertex3f(20.0f, -8.0f, i);
    }
    glEnd();
    
    // Water surface effect
    glColor4f(0.3f, 0.5f, 0.6f, 0.15f);
    glBegin(GL_QUADS);
    float waveHeight = 0.2f;
    for (int i = -30; i < 30; i++) {
        float wave = sinf(globalTime * 2.0f + i * 0.3f) * waveHeight;
        glVertex3f(i, 8.0f + wave, -5.0f);
        glVertex3f(i + 1, 8.0f + wave, -5.0f);
        glVertex3f(i + 1, 8.0f + wave, 5.0f);
        glVertex3f(i, 8.0f + wave, 5.0f);
    }
    glEnd();
    
    glEnable(GL_LIGHTING);
}

// Draw a single fin (equilateral triangle)
void drawFin(float scale) {
    glBegin(GL_TRIANGLES);
    glColor4f(0.3f, 0.6f, 0.5f, 0.6f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, scale, 0.0f);
    glVertex3f(0.0f, 0.0f, scale);
    glEnd();
}

// Draw the complete fish
void drawFish(float x, float y, float z) {
    glColor4f(0.4f, 0.7f, 0.6f, 0.85f);
    
    // Calculate tail swing based on position along body
    float tailWave = sinf(globalTime * 5.0f) * 0.3f;
    
    // HEAD
    glPushMatrix();
    glTranslatef(x + 1.2f, y, z);
    glColor4f(0.5f, 0.75f, 0.65f, 0.9f);
    glutSolidSphere(0.35f, 20, 20);
    
    // Eye
    glPushMatrix();
    glTranslatef(0.25f, 0.15f, 0.25f);
    glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
    glutSolidSphere(0.08f, 10, 10);
    glPopMatrix();
    glPopMatrix();
    
    // BODY (main scaled sphere)
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(2.5f, 1.0f, 1.0f);
    glColor4f(0.4f, 0.7f, 0.6f, 0.85f);
    glutSolidSphere(0.5f, 30, 30);
    glPopMatrix();
    
    // TAIL
    glPushMatrix();
    glTranslatef(x - 1.3f, y, z);
    glRotatef(tailWave * 25.0f, 0.0f, 1.0f, 0.0f);
    glColor4f(0.35f, 0.65f, 0.55f, 0.8f);
    glutSolidCone(0.25f, 0.8f, 15, 15);
    glPopMatrix();
    
    // TOP FIN (dorsal)
    glPushMatrix();
    glTranslatef(x - 0.3f, y + 0.6f, z);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glColor4f(0.3f, 0.6f, 0.5f, 0.6f);
    drawFin(0.4f);
    glPopMatrix();
    
    // BOTTOM FIN (anal)
    glPushMatrix();
    glTranslatef(x - 0.3f, y - 0.6f, z);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glColor4f(0.3f, 0.6f, 0.5f, 0.6f);
    drawFin(0.35f);
    glPopMatrix();
    
    // RIGHT PECTORAL FIN
    glPushMatrix();
    glTranslatef(x + 0.3f, y - 0.4f, z + 0.6f);
    glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(sinf(globalTime * 4.0f) * 20.0f, 1.0f, 0.0f, 0.0f);
    glColor4f(0.3f, 0.6f, 0.5f, 0.6f);
    drawFin(0.3f);
    glPopMatrix();
    
    // LEFT PECTORAL FIN
    glPushMatrix();
    glTranslatef(x + 0.3f, y - 0.4f, z - 0.6f);
    glRotatef(-60.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(sinf(globalTime * 4.0f) * 20.0f, 1.0f, 0.0f, 0.0f);
    glColor4f(0.3f, 0.6f, 0.5f, 0.6f);
    drawFin(0.3f);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera follows fish from side
    gluLookAt(
        fishX + 3.0f, 1.5f, 6.0f,
        fishX, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );
    
    // Update fish position
    fishX += fishSpeed * 0.01f;
    
    // Add wave motion to fish body
    fishY = sinf(globalTime * 1.2f) * 0.3f;
    fishZ = cosf(globalTime * 0.8f) * 0.2f;
    
    // Draw environment
    drawUnderwater();
    drawParticles();
    
    // Draw fish
    drawFish(fishX, fishY, fishZ);
    
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
    glColor4f(0.7f, 0.9f, 1.0f, 0.95f);
    glRasterPos2f(50, 50);
    const char* title = "Chapter 11: Early Fish - First Aquatic Vertebrate";
    for (const char* c = title; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
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

void init() {
    glClearColor(0.1f, 0.2f, 0.25f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Light setup (sun from above)
    GLfloat light_position[] = { 5.0f, 8.0f, 5.0f, 0.0f };
    GLfloat light_ambient[] = { 0.3f, 0.4f, 0.45f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.85f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Material properties
    GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat material_shininess[] = { 32.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
    
    initializeParticles();
}

void timer(int value) {
    globalTime += 0.016f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Chapter 11: Early Fish - First Aquatic Vertebrate");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glutMainLoop();
    
    return 0;
}
