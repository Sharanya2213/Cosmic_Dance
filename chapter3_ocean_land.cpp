#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>

float globalTime = 0.0f;
float fishX = -8.0f;
float fishY = 0.5f;
float fishZ = 0.0f;
float fishSpeed = 0.02f;

// Simple vector for plant positions
struct Plant {
    float x, z;
};

std::vector<Plant> plants;

void initializePlants() {
    plants.clear();
    // Create plants on land (right side)
    for (int i = 0; i < 8; i++) {
        float plantX = 2.0f + (rand() % 60) / 10.0f;
        float plantZ = -3.0f + (rand() % 60) / 10.0f;
        plants.push_back({plantX, plantZ});
    }
}

// Draw a simple plant (stem + leaves)
void drawPlant(float x, float z, float time) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    
    // Stem (cylinder)
    GLUquadric* quad = gluNewQuadric();
    glColor4f(0.3f, 0.6f, 0.2f, 0.9f);
    
    // Slight sway
    float sway = sinf(time * 1.5f + x * 0.1f) * 5.0f;
    glRotatef(sway, 0.0f, 0.0f, 1.0f);
    
    gluCylinder(quad, 0.1f, 0.08f, 0.8f, 8, 8);
    
    // Leaves (simple triangles arranged around stem)
    glDisable(GL_LIGHTING);
    glColor4f(0.2f, 0.8f, 0.2f, 0.85f);
    
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        float angle = (i / 4.0f) * 6.283f;
        glRotatef(angle * 57.3f, 0.0f, 1.0f, 0.0f);
        
        glBegin(GL_TRIANGLES);
        glVertex3f(0.1f, 0.4f, 0.0f);
        glVertex3f(0.3f, 0.7f, 0.1f);
        glVertex3f(0.0f, 0.8f, 0.05f);
        glEnd();
        
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// Draw water plane with waves
void drawOcean() {
    glColor4f(0.1f, 0.3f, 0.6f, 0.7f);
    glBegin(GL_QUADS);
    
    // Water surface with waves
    for (float x = -10.0f; x < 0.0f; x += 0.5f) {
        float waveY1 = sinf(x + globalTime) * 0.3f;
        float waveY2 = sinf(x + 0.5f + globalTime) * 0.3f;
        float waveY3 = sinf(x + 0.5f + globalTime) * 0.3f;
        float waveY4 = sinf(x + globalTime) * 0.3f;
        
        glVertex3f(x, waveY1, -5.0f);
        glVertex3f(x + 0.5f, waveY2, -5.0f);
        glVertex3f(x + 0.5f, waveY3, 5.0f);
        glVertex3f(x, waveY4, 5.0f);
    }
    glEnd();
    
    // Ocean floor (darker blue below)
    glColor4f(0.05f, 0.15f, 0.4f, 0.8f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, -2.0f, -5.0f);
    glVertex3f(0.0f, -2.0f, -5.0f);
    glVertex3f(0.0f, -2.0f, 5.0f);
    glVertex3f(-10.0f, -2.0f, 5.0f);
    glEnd();
}

// Draw shore (transition zone)
void drawShore() {
    glColor4f(0.8f, 0.7f, 0.4f, 0.9f);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, 0.1f, -5.0f);
    glVertex3f(1.5f, -0.1f, -5.0f);
    glVertex3f(1.5f, -0.1f, 5.0f);
    glVertex3f(-0.5f, 0.1f, 5.0f);
    glEnd();
}

// Draw land plane
void drawLand() {
    glColor4f(0.2f, 0.7f, 0.2f, 0.9f);
    glBegin(GL_QUADS);
    glVertex3f(1.5f, -0.0f, -5.0f);
    glVertex3f(10.0f, -0.0f, -5.0f);
    glVertex3f(10.0f, -0.0f, 5.0f);
    glVertex3f(1.5f, -0.0f, 5.0f);
    glEnd();
}

// Draw fish
void drawFish(float x, float y, float z, bool isWalking) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // HEAD
    glPushMatrix();
    glTranslatef(0.4f, 0.0f, 0.0f);
    glColor4f(0.5f, 0.7f, 0.6f, 0.9f);
    glutSolidSphere(0.25f, 16, 16);
    
    // Eye
    glPushMatrix();
    glTranslatef(0.15f, 0.1f, 0.15f);
    glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
    glutSolidSphere(0.06f, 8, 8);
    glPopMatrix();
    glPopMatrix();
    
    // BODY
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    float bodyScaleX = isWalking ? 1.3f : 2.5f;
    float bodyScaleY = isWalking ? 1.1f : 0.7f;
    glScalef(bodyScaleX, bodyScaleY, 1.0f);
    glColor4f(0.4f, 0.7f, 0.6f, 0.85f);
    glutSolidSphere(0.4f, 20, 20);
    glPopMatrix();
    
    // TAIL (shrinks when walking)
    if (!isWalking) {
        glPushMatrix();
        glTranslatef(-0.5f, 0.0f, 0.0f);
        float tailWave = sinf(globalTime * 5.0f) * 0.3f;
        glRotatef(tailWave * 25.0f, 0.0f, 1.0f, 0.0f);
        glColor4f(0.35f, 0.65f, 0.55f, 0.8f);
        glutSolidCone(0.2f, 0.6f, 12, 12);
        glPopMatrix();
    } else {
        // Small tail vestige
        glPushMatrix();
        glTranslatef(-0.5f, 0.0f, 0.0f);
        glColor4f(0.35f, 0.65f, 0.55f, 0.6f);
        glutSolidSphere(0.1f, 8, 8);
        glPopMatrix();
    }
    
    // WALKING LEGS (when on land)
    if (isWalking) {
        GLUquadric* quad = gluNewQuadric();
        glColor4f(0.2f, 0.5f, 0.4f, 0.95f);
        
        // Front left leg - swings forward dramatically
        glPushMatrix();
        glTranslatef(0.25f, -0.3f, 0.35f);
        float legSwing1 = sinf(globalTime * 5.0f) * 60.0f;
        glRotatef(legSwing1, 1.0f, 0.0f, 0.0f);
        gluCylinder(quad, 0.12f, 0.1f, 0.6f, 10, 10);
        // Foot
        glTranslatef(0.0f, -0.6f, 0.0f);
        glutSolidSphere(0.15f, 10, 10);
        glPopMatrix();
        
        // Front right leg - opposite phase
        glPushMatrix();
        glTranslatef(0.25f, -0.3f, -0.35f);
        float legSwing2 = sinf(globalTime * 5.0f + 3.14159f) * 60.0f;
        glRotatef(legSwing2, 1.0f, 0.0f, 0.0f);
        gluCylinder(quad, 0.12f, 0.1f, 0.6f, 10, 10);
        // Foot
        glTranslatef(0.0f, -0.6f, 0.0f);
        glutSolidSphere(0.15f, 10, 10);
        glPopMatrix();
        
        // Rear left leg - opposite phase to front left
        glPushMatrix();
        glTranslatef(-0.25f, -0.3f, 0.35f);
        float legSwing3 = sinf(globalTime * 5.0f + 3.14159f) * 60.0f;
        glRotatef(legSwing3, 1.0f, 0.0f, 0.0f);
        gluCylinder(quad, 0.12f, 0.1f, 0.6f, 10, 10);
        // Foot
        glTranslatef(0.0f, -0.6f, 0.0f);
        glutSolidSphere(0.15f, 10, 10);
        glPopMatrix();
        
        // Rear right leg - opposite phase to front right
        glPushMatrix();
        glTranslatef(-0.25f, -0.3f, -0.35f);
        float legSwing4 = sinf(globalTime * 5.0f) * 60.0f;
        glRotatef(legSwing4, 1.0f, 0.0f, 0.0f);
        gluCylinder(quad, 0.12f, 0.1f, 0.6f, 10, 10);
        // Foot
        glTranslatef(0.0f, -0.6f, 0.0f);
        glutSolidSphere(0.15f, 10, 10);
        glPopMatrix();
    }
    
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Update fish position and type
    bool isWalking = fishX > 0.2f;
    
    // Camera zoomed out for full scene view
    float cameraDistance = 8.0f;
    float cameraHeight = 3.5f;
    gluLookAt(
        2.0f, cameraHeight, cameraDistance,
        2.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );
    
    // Stop animation at end of ocean/beginning of shore (around x = 1.5)
    if (fishX < 1.5f) {
        if (!isWalking) {
            // Swimming in water
            fishX += fishSpeed;
            fishY = sinf(globalTime * 1.5f + fishX * 2.0f) * 0.2f;
        } else {
            // Walking on land
            fishX += fishSpeed * 0.6f;
            fishY = -0.1f + sinf(globalTime * 2.8f) * 0.08f;  // Walk on ground with body bob
        }
    }
    // If fishX >= 1.5, fish stays in final position (animation ends)
    
    // Draw environment
    drawOcean();
    drawShore();
    drawLand();
    
    // Draw plants
    for (const auto& plant : plants) {
        drawPlant(plant.x, plant.z, globalTime);
    }
    
    // Draw fish
    drawFish(fishX, fishY, fishZ, isWalking);
    
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
    const char* title = "Life Moves from Ocean to Land";
    for (const char* c = title; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    
    glRasterPos2f(50, 80);
    const char* status = isWalking ? "Status: WALKING ON LAND" : "Status: SWIMMING IN OCEAN";
    for (const char* c = status; *c; c++) {
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
    
    GLfloat light_position[] = { 5.0f, 8.0f, 5.0f, 0.0f };
    GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
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
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);  // Sky blue
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    setupLighting();
    initializePlants();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Chapter 3: Life Moves from Ocean to Land");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}
