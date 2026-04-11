#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>

float globalTime = 0.0f;

// Dinosaur structure
struct Dinosaur {
    float x, z;
    float scale;
    float speed;
    int type; // 0=TRex, 1=Triceratops, 2=Stegosaurus
};

// Vegetation
struct Plant {
    float x, z;
    float height;
};

std::vector<Dinosaur> dinosaurs;
std::vector<Plant> plants;

void initializeDinosaurs() {
    dinosaurs.clear();
    dinosaurs.push_back({-8.0f, 0.0f, 1.2f, 0.015f, 0}); // T-Rex
    dinosaurs.push_back({-5.0f, 3.0f, 0.9f, 0.01f, 1});  // Triceratops
    dinosaurs.push_back({2.0f, -2.0f, 1.0f, 0.008f, 2}); // Stegosaurus
}

void initializePlants() {
    plants.clear();
    for (int i = 0; i < 15; i++) {
        float x = -10.0f + (rand() % 200) / 10.0f;
        float z = -5.0f + (rand() % 100) / 10.0f;
        float h = 1.0f + (rand() % 30) / 100.0f;
        plants.push_back({x, z, h});
    }
}

// Draw fern-like plant
void drawFern(float x, float z, float height) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    
    // Trunk
    GLUquadric* quad = gluNewQuadric();
    glColor4f(0.3f, 0.5f, 0.1f, 0.9f);
    gluCylinder(quad, 0.06f, 0.04f, height * 0.4f, 6, 6);
    
    // Fronds
    glTranslatef(0.0f, height * 0.4f, 0.0f);
    glColor4f(0.2f, 0.7f, 0.15f, 0.8f);
    
    float sway = sinf(globalTime * 0.8f + x * 0.1f) * 2.0f;
    glRotatef(sway, 0.0f, 0.0f, 1.0f);
    
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glRotatef((i / 3.0f) * 120.0f, 0.0f, 1.0f, 0.0f);
        glutSolidCone(height * 0.2f, height * 0.6f, 12, 8);
        glPopMatrix();
    }
    
    glPopMatrix();
}

// Draw T-Rex
void drawTRex(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Head
    glPushMatrix();
    glTranslatef(2.0f, 1.5f, 0.0f);
    glColor4f(0.7f, 0.5f, 0.3f, 0.9f);
    glutSolidSphere(0.5f, 16, 16);
    
    // Jaw
    glTranslatef(0.3f, -0.2f, 0.0f);
    glColor4f(0.6f, 0.4f, 0.2f, 0.9f);
    glScalef(1.2f, 0.6f, 0.8f);
    glutSolidSphere(0.3f, 12, 12);
    glPopMatrix();
    
    // Body
    glPushMatrix();
    glTranslatef(0.5f, 0.8f, 0.0f);
    glColor4f(0.6f, 0.4f, 0.2f, 0.9f);
    glScalef(3.0f, 1.2f, 1.0f);
    glutSolidSphere(0.6f, 20, 20);
    glPopMatrix();
    
    // Tail (swinging)
    glPushMatrix();
    glTranslatef(-1.5f, 0.6f, 0.0f);
    float tailWave = sinf(globalTime * 3.0f) * 25.0f;
    glRotatef(tailWave, 0.0f, 1.0f, 0.0f);
    glColor4f(0.5f, 0.3f, 0.15f, 0.85f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.3f, 0.15f, 2.5f, 12, 12);
    glPopMatrix();
    
    // Front legs (tiny)
    glPushMatrix();
    glTranslatef(1.2f, 0.3f, 0.5f);
    glColor4f(0.5f, 0.3f, 0.15f, 0.9f);
    glScalef(0.3f, 0.8f, 0.3f);
    glutSolidSphere(0.2f, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.2f, 0.3f, -0.5f);
    glColor4f(0.5f, 0.3f, 0.15f, 0.9f);
    glScalef(0.3f, 0.8f, 0.3f);
    glutSolidSphere(0.2f, 8, 8);
    glPopMatrix();
    
    // Hind legs
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.7f);
    glColor4f(0.4f, 0.25f, 0.1f, 0.9f);
    glScalef(0.5f, 1.5f, 0.4f);
    glutSolidSphere(0.25f, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.7f);
    glColor4f(0.4f, 0.25f, 0.1f, 0.9f);
    glScalef(0.5f, 1.5f, 0.4f);
    glutSolidSphere(0.25f, 10, 10);
    glPopMatrix();
    
    glPopMatrix();
}

// Draw Triceratops
void drawTriceratops(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Head with horns
    glPushMatrix();
    glTranslatef(1.5f, 1.2f, 0.0f);
    glColor4f(0.6f, 0.5f, 0.3f, 0.9f);
    glutSolidSphere(0.6f, 16, 16);
    
    // Horn 1 (top)
    glPushMatrix();
    glTranslatef(0.3f, 0.5f, 0.0f);
    glColor4f(0.8f, 0.7f, 0.5f, 0.9f);
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f, 0.6f, 8, 8);
    glPopMatrix();
    
    // Horn 2 (left)
    glPushMatrix();
    glTranslatef(0.1f, 0.2f, 0.4f);
    glColor4f(0.8f, 0.7f, 0.5f, 0.9f);
    glRotatef(40.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.06f, 0.5f, 8, 8);
    glPopMatrix();
    
    // Horn 3 (right)
    glPushMatrix();
    glTranslatef(0.1f, 0.2f, -0.4f);
    glColor4f(0.8f, 0.7f, 0.5f, 0.9f);
    glRotatef(40.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.06f, 0.5f, 8, 8);
    glPopMatrix();
    
    // Frill (neck)
    glTranslatef(-0.2f, 0.3f, 0.0f);
    glColor4f(0.5f, 0.4f, 0.2f, 0.85f);
    glScalef(1.2f, 0.8f, 1.8f);
    glutSolidSphere(0.4f, 14, 14);
    glPopMatrix();
    
    // Body
    glPushMatrix();
    glTranslatef(0.2f, 0.7f, 0.0f);
    glColor4f(0.5f, 0.45f, 0.25f, 0.9f);
    glScalef(2.5f, 1.3f, 1.2f);
    glutSolidSphere(0.7f, 18, 18);
    glPopMatrix();
    
    // Tail
    glPushMatrix();
    glTranslatef(-1.2f, 0.5f, 0.0f);
    glColor4f(0.4f, 0.35f, 0.15f, 0.85f);
    GLUquadric* quad2 = gluNewQuadric();
    gluCylinder(quad2, 0.25f, 0.1f, 1.8f, 10, 10);
    glPopMatrix();
    
    // Legs
    for (float zOff : {-0.6f, 0.6f}) {
        glPushMatrix();
        glTranslatef(0.5f, 0.0f, zOff);
        glColor4f(0.4f, 0.35f, 0.15f, 0.9f);
        glScalef(0.4f, 1.8f, 0.4f);
        glutSolidSphere(0.28f, 10, 10);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-0.5f, 0.0f, zOff);
        glColor4f(0.4f, 0.35f, 0.15f, 0.9f);
        glScalef(0.4f, 1.8f, 0.4f);
        glutSolidSphere(0.28f, 10, 10);
        glPopMatrix();
    }
    
    glPopMatrix();
}

// Draw Stegosaurus
void drawStegosaurus(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Head
    glPushMatrix();
    glTranslatef(1.5f, 1.0f, 0.0f);
    glColor4f(0.5f, 0.6f, 0.3f, 0.9f);
    glutSolidSphere(0.4f, 14, 14);
    glPopMatrix();
    
    // Body with plates on back
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.0f);
    glColor4f(0.4f, 0.55f, 0.25f, 0.9f);
    glScalef(3.0f, 1.0f, 1.1f);
    glutSolidSphere(0.65f, 20, 20);
    glPopMatrix();
    
    // Dorsal plates along back
    glColor4f(0.7f, 0.4f, 0.2f, 0.8f);
    for (int i = 0; i < 8; i++) {
        glPushMatrix();
        float posX = -0.8f + (i / 7.0f) * 2.0f;
        glTranslatef(posX, 1.5f, 0.0f);
        
        float plateSway = sinf(globalTime * 1.5f + i) * 0.1f;
        glRotatef(plateSway * 20.0f, 0.0f, 0.0f, 1.0f);
        
        glScalef(0.3f, 0.9f, 0.2f);
        glutSolidSphere(0.35f, 10, 10);
        glPopMatrix();
    }
    
    // Tail with spikes
    glPushMatrix();
    glTranslatef(-1.8f, 0.6f, 0.0f);
    float tailWave = sinf(globalTime * 2.5f) * 20.0f;
    glRotatef(tailWave, 0.0f, 1.0f, 0.0f);
    glColor4f(0.35f, 0.5f, 0.2f, 0.85f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.3f, 0.1f, 2.0f, 12, 12);
    
    // Tail spikes
    glColor4f(0.6f, 0.3f, 0.1f, 0.85f);
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        float spikePos = 0.5f + (i / 3.0f) * 1.5f;
        glTranslatef(0.0f, 0.0f, spikePos);
        float spikeAngle = (i % 2 == 0) ? 30.0f : -30.0f;
        glRotatef(spikeAngle, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.1f, 0.5f, 8, 8);
        glPopMatrix();
    }
    glPopMatrix();
    
    // Legs
    for (float zOff : {-0.7f, 0.7f}) {
        for (float xPos : {0.5f, -0.5f}) {
            glPushMatrix();
            glTranslatef(xPos, 0.0f, zOff);
            glColor4f(0.35f, 0.5f, 0.2f, 0.9f);
            glScalef(0.45f, 1.6f, 0.45f);
            glutSolidSphere(0.3f, 10, 10);
            glPopMatrix();
        }
    }
    
    glPopMatrix();
}

void drawGround() {
    glColor4f(0.2f, 0.6f, 0.15f, 0.95f);
    glBegin(GL_QUADS);
    glVertex3f(-15.0f, -0.05f, -8.0f);
    glVertex3f(15.0f, -0.05f, -8.0f);
    glVertex3f(15.0f, -0.05f, 8.0f);
    glVertex3f(-15.0f, -0.05f, 8.0f);
    glEnd();
    
    // Ground grid
    glColor4f(0.3f, 0.7f, 0.2f, 0.6f);
    glBegin(GL_LINES);
    for (float x = -15.0f; x <= 15.0f; x += 3.0f) {
        glVertex3f(x, 0.0f, -8.0f);
        glVertex3f(x, 0.0f, 8.0f);
    }
    for (float z = -8.0f; z <= 8.0f; z += 3.0f) {
        glVertex3f(-15.0f, 0.0f, z);
        glVertex3f(15.0f, 0.0f, z);
    }
    glEnd();
}

void drawSky() {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    glBegin(GL_QUADS);
    // Hazy sky
    glColor4f(0.7f, 0.8f, 0.5f, 1.0f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glColor4f(0.3f, 0.5f, 0.7f, 1.0f);
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
    
    // Camera overview of prehistoric landscape
    gluLookAt(
        0.0f, 5.0f, 12.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );
    
    drawGround();
    
    // Draw plants
    for (const auto& plant : plants) {
        drawFern(plant.x, plant.z, plant.height);
    }
    
    // Update and draw dinosaurs
    for (auto& dino : dinosaurs) {
        dino.x += dino.speed;
        
        // Wrap around
        if (dino.x > 15.0f) dino.x = -15.0f;
        
        if (dino.type == 0) {
            drawTRex(dino.x, dino.z, dino.scale);
        } else if (dino.type == 1) {
            drawTriceratops(dino.x, dino.z, dino.scale);
        } else {
            drawStegosaurus(dino.x, dino.z, dino.scale);
        }
    }
    
    // Text overlay
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
    const char* title = "Chapter 14: Age of Dinosaurs";
    for (const char* c = title; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    
    glRasterPos2f(50, 80);
    char timeStr[50];
    sprintf(timeStr, "Time: %.1f seconds", globalTime);
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
    
    GLfloat light_position[] = {8.0f, 10.0f, 8.0f, 0.0f};
    GLfloat light_ambient[] = {0.5f, 0.5f, 0.4f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 0.9f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    GLfloat material_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat material_shininess[] = {32.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
}

void init() {
    glClearColor(0.7f, 0.8f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    setupLighting();
    initializeDinosaurs();
    initializePlants();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Chapter 14: Age of Dinosaurs");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}
