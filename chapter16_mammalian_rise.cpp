#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>

float globalTime = 0.0f;

struct Mammal {
    float x, z;
    float scale;
    float speed;
    int type; // 0=early mammal, 1=small primate, 2=herbivore
};

struct Plant {
    float x, z;
    float height;
};

std::vector<Mammal> mammals;
std::vector<Plant> plants;

void initializeMammals() {
    mammals.clear();
    mammals.push_back({-8.0f, 0.0f, 0.5f, 0.012f, 0});  // Early mammal
    mammals.push_back({-4.0f, 2.0f, 0.4f, 0.015f, 1});  // Primate
    mammals.push_back({3.0f, -2.0f, 0.6f, 0.01f, 2});   // Herbivore
    mammals.push_back({2.0f, 3.0f, 0.35f, 0.014f, 1});  // Another primate
}

void initializePlants() {
    plants.clear();
    for (int i = 0; i < 20; i++) {
        float x = -10.0f + (rand() % 200) / 10.0f;
        float z = -5.0f + (rand() % 100) / 10.0f;
        float h = 0.8f + (rand() % 40) / 100.0f;
        plants.push_back({x, z, h});
    }
}

void drawFlower(float x, float z, float height) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    
    // Stem
    GLUquadric* quad = gluNewQuadric();
    glColor4f(0.2f, 0.6f, 0.2f, 0.9f);
    gluCylinder(quad, 0.04f, 0.02f, height * 0.6f, 6, 6);
    
    // Flower petals
    glTranslatef(0.0f, height * 0.6f, 0.0f);
    glColor4f(1.0f, 0.3f + sinf(globalTime) * 0.2f, 0.2f, 0.85f);
    
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        float angle = (i / 5.0f) * 6.283f;
        glRotatef(angle * 57.3f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.2f, 0.0f, 0.0f);
        glutSolidSphere(0.12f, 8, 8);
        glPopMatrix();
    }
    
    // Center of flower
    glColor4f(1.0f, 1.0f, 0.2f, 0.9f);
    glutSolidSphere(0.1f, 6, 6);
    
    glPopMatrix();
}

void drawEarlyMammal(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Body (rounded)
    glPushMatrix();
    glColor4f(0.5f, 0.55f, 0.5f, 0.9f);
    glScalef(1.5f, 0.9f, 0.8f);
    glutSolidSphere(0.5f, 16, 16);
    glPopMatrix();
    
    // Head
    glPushMatrix();
    glTranslatef(0.8f, 0.3f, 0.0f);
    glColor4f(0.55f, 0.6f, 0.55f, 0.9f);
    glutSolidSphere(0.25f, 12, 12);
    
    // Ear
    glTranslatef(0.15f, 0.25f, 0.15f);
    glColor4f(0.6f, 0.4f, 0.35f, 0.9f);
    glutSolidSphere(0.08f, 8, 8);
    glPopMatrix();
    
    // Tail (furry)
    glPushMatrix();
    glTranslatef(-0.8f, 0.2f, 0.0f);
    float tailWave = sinf(globalTime * 4.0f) * 20.0f;
    glRotatef(tailWave, 0.0f, 1.0f, 0.0f);
    glColor4f(0.4f, 0.45f, 0.4f, 0.85f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.12f, 0.06f, 1.2f, 10, 10);
    glPopMatrix();
    
    // Four short legs
    glColor4f(0.45f, 0.5f, 0.45f, 0.9f);
    for (float zPos : {-0.35f, 0.35f}) {
        for (float xPos : {0.3f, -0.3f}) {
            glPushMatrix();
            glTranslatef(xPos, -0.5f, zPos);
            glScalef(0.25f, 1.2f, 0.25f);
            glutSolidSphere(0.15f, 8, 8);
            glPopMatrix();
        }
    }
    
    glPopMatrix();
}

void drawPrimate(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Body
    glPushMatrix();
    glColor4f(0.6f, 0.5f, 0.4f, 0.9f);
    glScalef(1.0f, 1.3f, 0.7f);
    glutSolidSphere(0.45f, 16, 16);
    glPopMatrix();
    
    // Head
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.0f);
    glColor4f(0.7f, 0.55f, 0.4f, 0.9f);
    glutSolidSphere(0.3f, 14, 14);
    
    // Eyes
    glPushMatrix();
    glTranslatef(-0.1f, 0.1f, 0.25f);
    glColor4f(0.2f, 0.2f, 0.2f, 0.9f);
    glutSolidSphere(0.05f, 6, 6);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.1f, 0.1f, 0.25f);
    glColor4f(0.2f, 0.2f, 0.2f, 0.9f);
    glutSolidSphere(0.05f, 6, 6);
    glPopMatrix();
    glPopMatrix();
    
    // Arms (hanging)
    glColor4f(0.55f, 0.45f, 0.35f, 0.9f);
    for (float zOff : {-0.2f, 0.2f}) {
        glPushMatrix();
        glTranslatef(-0.4f, 0.3f, zOff);
        float armSway = sinf(globalTime * 2.5f + zOff) * 15.0f;
        glRotatef(armSway, 1.0f, 0.0f, 0.0f);
        glScalef(0.2f, 1.0f, 0.2f);
        glutSolidSphere(0.2f, 8, 8);
        glPopMatrix();
    }
    
    // Legs
    glColor4f(0.5f, 0.4f, 0.3f, 0.9f);
    for (float zOff : {-0.25f, 0.25f}) {
        glPushMatrix();
        glTranslatef(0.0f, -0.7f, zOff);
        glScalef(0.3f, 1.0f, 0.3f);
        glutSolidSphere(0.2f, 8, 8);
        glPopMatrix();
    }
    
    // Tail
    glPushMatrix();
    glTranslatef(0.0f, -0.2f, -0.4f);
    float tailCurve = sinf(globalTime * 3.0f) * 25.0f;
    glRotatef(tailCurve, 0.0f, 0.0f, 1.0f);
    glColor4f(0.5f, 0.4f, 0.3f, 0.8f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.1f, 0.05f, 1.0f, 8, 8);
    glPopMatrix();
    
    glPopMatrix();
}

void drawHerbivore(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Body
    glPushMatrix();
    glColor4f(0.7f, 0.6f, 0.4f, 0.9f);
    glScalef(1.8f, 1.1f, 0.9f);
    glutSolidSphere(0.5f, 18, 18);
    glPopMatrix();
    
    // Head
    glPushMatrix();
    glTranslatef(0.9f, 0.5f, 0.0f);
    glColor4f(0.75f, 0.65f, 0.45f, 0.9f);
    glScalef(1.2f, 0.8f, 1.0f);
    glutSolidSphere(0.3f, 14, 14);
    
    // Ear
    glPushMatrix();
    glTranslatef(0.25f, 0.35f, 0.3f);
    glColor4f(0.7f, 0.5f, 0.3f, 0.9f);
    glScalef(0.6f, 1.2f, 0.3f);
    glutSolidSphere(0.12f, 8, 8);
    glPopMatrix();
    glPopMatrix();
    
    // Antlers (small spikes)
    glColor4f(0.5f, 0.45f, 0.4f, 0.9f);
    glPushMatrix();
    glTranslatef(0.75f, 1.0f, 0.2f);
    glutSolidCone(0.08f, 0.35f, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.75f, 1.0f, -0.2f);
    glutSolidCone(0.08f, 0.35f, 8, 8);
    glPopMatrix();
    
    // Tail
    glPushMatrix();
    glTranslatef(-1.0f, 0.3f, 0.0f);
    glColor4f(0.6f, 0.5f, 0.35f, 0.85f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.15f, 0.08f, 0.9f, 10, 10);
    glPopMatrix();
    
    // Legs
    glColor4f(0.55f, 0.45f, 0.3f, 0.9f);
    for (float zOff : {-0.4f, 0.4f}) {
        for (float xPos : {0.3f, -0.4f}) {
            glPushMatrix();
            glTranslatef(xPos, -0.6f, zOff);
            glScalef(0.35f, 1.5f, 0.35f);
            glutSolidSphere(0.22f, 10, 10);
            glPopMatrix();
        }
    }
    
    glPopMatrix();
}

void drawGround() {
    glColor4f(0.2f, 0.6f, 0.15f, 0.95f);
    glBegin(GL_QUADS);
    glVertex3f(-15.0f, -0.05f, -5.0f);
    glVertex3f(15.0f, -0.05f, -5.0f);
    glVertex3f(15.0f, -0.05f, 5.0f);
    glVertex3f(-15.0f, -0.05f, 5.0f);
    glEnd();
    
    // Lush grass pattern
    glColor4f(0.15f, 0.55f, 0.1f, 0.7f);
    glBegin(GL_LINES);
    for (float x = -15.0f; x <= 15.0f; x += 2.0f) {
        glVertex3f(x, 0.0f, -5.0f);
        glVertex3f(x, 0.0f, 5.0f);
    }
    for (float z = -5.0f; z <= 5.0f; z += 2.0f) {
        glVertex3f(-15.0f, 0.0f, z);
        glVertex3f(15.0f, 0.0f, z);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(
        0.0f, 3.0f, 10.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );
    
    drawGround();
    
    // Draw plants and flowers
    for (const auto& plant : plants) {
        drawFlower(plant.x, plant.z, plant.height);
    }
    
    // Update and draw mammals
    for (auto& mammal : mammals) {
        mammal.x += mammal.speed;
        
        if (mammal.x > 15.0f) mammal.x = -15.0f;
        
        if (mammal.type == 0) {
            drawEarlyMammal(mammal.x, mammal.z, mammal.scale);
        } else if (mammal.type == 1) {
            drawPrimate(mammal.x, mammal.z, mammal.scale);
        } else {
            drawHerbivore(mammal.x, mammal.z, mammal.scale);
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
    const char* title = "Chapter 16: Mammalian Rise - Age of Warm-Blooded Life";
    for (const char* c = title; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    
    glRasterPos2f(50, 80);
    char info[] = "Small mammals diversify after extinction event";
    for (const char* c = info; *c; c++) {
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
    GLfloat light_ambient[] = {0.6f, 0.6f, 0.5f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 0.95f, 1.0f};
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
    glClearColor(0.6f, 0.8f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    setupLighting();
    initializeMammals();
    initializePlants();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Chapter 16: Mammalian Rise");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}
