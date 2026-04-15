#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdio>

float globalTime = 0.0f;
bool asteroidActive = false;
float asteroidX = 80.0f;
float asteroidY = 60.0f;
float asteroidZ = 0.0f;
float blastRadius = 0.0f;
float blastIntensity = 0.0f;

struct Dinosaur {
    float x, z;
    float scale;
    float speed;
    int type;  // 0=T-Rex, 1=Triceratops
};

std::vector<Dinosaur> dinosaurs;

void initDinosaurs() {
    dinosaurs.clear();
    dinosaurs.push_back({-5.0f, 0.0f, 1.2f, 0.015f, 0});  // T-Rex
    dinosaurs.push_back({0.0f, 2.0f, 0.9f, 0.012f, 0});    // T-Rex
    dinosaurs.push_back({8.0f, -1.0f, 1.0f, 0.01f, 1});    // Triceratops
    printf("Initialized %d dinosaurs\n", (int)dinosaurs.size());
}

void drawSky() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Gray sky gradient
    glBegin(GL_QUADS);
    // Dark gray top
    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-100.0f, 40.0f, -100.0f);
    glVertex3f(100.0f, 40.0f, -100.0f);
    
    // Light gray bottom
    glColor3f(0.6f, 0.6f, 0.6f);
    glVertex3f(100.0f, 5.0f, -100.0f);
    glVertex3f(-100.0f, 5.0f, -100.0f);
    glEnd();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void drawGround() {
    // Main green grass area
    glColor3f(0.25f, 0.55f, 0.15f);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glEnd();
    
    // Grass texture detail
    glColor3f(0.2f, 0.5f, 0.1f);
    glBegin(GL_LINES);
    glLineWidth(0.5f);
    for (float x = -100.0f; x < 100.0f; x += 5.0f) {
        for (float z = -100.0f; z < 100.0f; z += 5.0f) {
            glVertex3f(x, 0.01f, z);
            glVertex3f(x + 2.5f, 0.01f, z + 2.5f);
        }
    }
    glLineWidth(1.0f);
    glEnd();
}

void drawTRex(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Body (large)
    glPushMatrix();
    glColor3f(0.6f, 0.4f, 0.2f);  // Brown
    glScalef(2.5f, 1.2f, 1.0f);
    glutSolidSphere(0.8f, 16, 16);
    glPopMatrix();
    
    // Neck
    glPushMatrix();
    glTranslatef(1.2f, 0.8f, 0.0f);
    glColor3f(0.65f, 0.45f, 0.25f);
    glRotatef(20.0f, 0.0f, 0.0f, 1.0f);
    glScalef(0.7f, 1.5f, 0.7f);
    glutSolidSphere(0.4f, 12, 12);
    glPopMatrix();
    
    // Head
    glPushMatrix();
    glTranslatef(1.8f, 1.6f, 0.0f);
    glColor3f(0.7f, 0.5f, 0.3f);
    glScalef(1.1f, 0.9f, 0.85f);
    glutSolidSphere(0.5f, 14, 14);
    
    // Jaw/mouth detail
    glColor3f(0.55f, 0.35f, 0.15f);
    glPushMatrix();
    glTranslatef(0.35f, -0.3f, 0.0f);
    glScalef(0.7f, 0.4f, 0.6f);
    glutSolidSphere(0.4f, 8, 8);
    glPopMatrix();
    
    glPopMatrix();
    
    // Eye
    glPushMatrix();
    glTranslatef(2.0f, 1.8f, 0.35f);
    glColor3f(0.8f, 0.7f, 0.5f);
    glutSolidSphere(0.15f, 8, 8);
    glPopMatrix();
    
    // Front left leg
    glPushMatrix();
    glTranslatef(0.5f, -0.7f, 0.6f);
    glColor3f(0.4f, 0.25f, 0.05f);
    glScalef(0.65f, 2.2f, 0.65f);
    glutSolidSphere(0.42f, 12, 12);
    glPopMatrix();
    
    // Front right leg
    glPushMatrix();
    glTranslatef(0.5f, -0.7f, -0.6f);
    glColor3f(0.4f, 0.25f, 0.05f);
    glScalef(0.65f, 2.2f, 0.65f);
    glutSolidSphere(0.42f, 12, 12);
    glPopMatrix();
    
    // Back left leg
    glPushMatrix();
    glTranslatef(-1.0f, -0.7f, 0.7f);
    glColor3f(0.4f, 0.25f, 0.05f);
    glScalef(0.75f, 2.4f, 0.75f);
    glutSolidSphere(0.45f, 12, 12);
    glPopMatrix();
    
    // Back right leg
    glPushMatrix();
    glTranslatef(-1.0f, -0.7f, -0.7f);
    glColor3f(0.4f, 0.25f, 0.05f);
    glScalef(0.75f, 2.4f, 0.75f);
    glutSolidSphere(0.45f, 12, 12);
    glPopMatrix();
    
    // Tail
    glPushMatrix();
    glTranslatef(-2.0f, 0.5f, 0.0f);
    float tailSway = sinf(globalTime * 3.0f) * 15.0f;
    glRotatef(tailSway, 0.0f, 0.0f, 1.0f);
    glColor3f(0.5f, 0.3f, 0.1f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.35f, 2.0f, 10, 10);
    glPopMatrix();
    
    // Small arms
    glColor3f(0.5f, 0.3f, 0.1f);
    glPushMatrix();
    glTranslatef(0.8f, 0.2f, 0.3f);
    glScalef(0.3f, 0.6f, 0.3f);
    glutSolidSphere(0.25f, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.8f, 0.2f, -0.3f);
    glScalef(0.3f, 0.6f, 0.3f);
    glutSolidSphere(0.25f, 8, 8);
    glPopMatrix();
    
    glPopMatrix();
}

void drawTriceratops(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Body
    glPushMatrix();
    glColor3f(0.65f, 0.45f, 0.25f);  // Brown
    glScalef(2.2f, 1.1f, 0.95f);
    glutSolidSphere(0.75f, 16, 16);
    glPopMatrix();
    
    // Neck/Head
    glPushMatrix();
    glTranslatef(1.0f, 0.6f, 0.0f);
    glColor3f(0.7f, 0.5f, 0.3f);
    glScalef(1.0f, 0.8f, 1.0f);
    glutSolidSphere(0.6f, 14, 14);
    glPopMatrix();
    
    // Front horn
    glPushMatrix();
    glTranslatef(1.5f, 1.3f, 0.0f);
    glColor3f(0.8f, 0.7f, 0.5f);
    glRotatef(25.0f, 0.0f, 0.0f, 1.0f);
    glutSolidCone(0.18f, 0.8f, 8, 8);
    glPopMatrix();
    
    // Left back horn
    glPushMatrix();
    glTranslatef(0.8f, 1.1f, 0.35f);
    glColor3f(0.75f, 0.65f, 0.45f);
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
    glutSolidCone(0.12f, 0.6f, 8, 8);
    glPopMatrix();
    
    // Right back horn
    glPushMatrix();
    glTranslatef(0.8f, 1.1f, -0.35f);
    glColor3f(0.75f, 0.65f, 0.45f);
    glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
    glutSolidCone(0.12f, 0.6f, 8, 8);
    glPopMatrix();
    
    // Front left leg
    glPushMatrix();
    glTranslatef(0.6f, -0.6f, 0.6f);
    glColor3f(0.45f, 0.3f, 0.1f);
    glScalef(0.65f, 2.1f, 0.65f);
    glutSolidSphere(0.4f, 12, 12);
    glPopMatrix();
    
    // Front right leg
    glPushMatrix();
    glTranslatef(0.6f, -0.6f, -0.6f);
    glColor3f(0.45f, 0.3f, 0.1f);
    glScalef(0.65f, 2.1f, 0.65f);
    glutSolidSphere(0.4f, 12, 12);
    glPopMatrix();
    
    // Back left leg
    glPushMatrix();
    glTranslatef(-0.8f, -0.6f, 0.7f);
    glColor3f(0.45f, 0.3f, 0.1f);
    glScalef(0.7f, 2.2f, 0.7f);
    glutSolidSphere(0.42f, 12, 12);
    glPopMatrix();
    
    // Back right leg
    glPushMatrix();
    glTranslatef(-0.8f, -0.6f, -0.7f);
    glColor3f(0.45f, 0.3f, 0.1f);
    glScalef(0.7f, 2.2f, 0.7f);
    glutSolidSphere(0.42f, 12, 12);
    glPopMatrix();
    
    // Tail
    glPushMatrix();
    glTranslatef(-1.8f, 0.3f, 0.0f);
    float tailSway = sinf(globalTime * 2.5f) * 12.0f;
    glRotatef(tailSway, 0.0f, 0.0f, 1.0f);
    glColor3f(0.55f, 0.35f, 0.15f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.3f, 1.8f, 10, 10);
    glPopMatrix();
    
    glPopMatrix();
}

void drawAsteroid() {
    if (!asteroidActive) return;
    
    glDisable(GL_LIGHTING);
    
    // Bright meteor core
    glColor3f(1.0f, 0.95f, 0.8f);
    glPushMatrix();
    glTranslatef(asteroidX, asteroidY, asteroidZ);
    glutSolidSphere(0.6f, 12, 12);
    glPopMatrix();
    
    // Glow halo
    glColor4f(1.0f, 0.7f, 0.2f, 0.7f);
    glPushMatrix();
    glTranslatef(asteroidX, asteroidY, asteroidZ);
    glutSolidSphere(1.5f, 10, 10);
    glPopMatrix();
    
    // Long trailing tail
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 15; i++) {
        float t = (float)i / 15.0f;
        float tx = asteroidX + (t * 8.0f);
        float ty = asteroidY + (t * 6.0f);
        
        float fade = 1.0f - (t * t);
        glColor4f(1.0f, 0.6f - (t * 0.3f), 0.1f, fade * 0.8f);
        glVertex3f(tx, ty, asteroidZ);
    }
    glEnd();
    glLineWidth(1.0f);
    
    glEnable(GL_LIGHTING);
}

void drawBlast() {
    if (blastRadius <= 0.01f) return;
    
    glDisable(GL_LIGHTING);
    
    // Expanding orange-red blast wave
    glColor4f(1.0f, 0.4f, 0.0f, blastIntensity * 0.8f);
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(blastRadius, 16, 16);
    glPopMatrix();
    
    // Inner bright core
    glColor4f(1.0f, 0.7f, 0.2f, blastIntensity);
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(blastRadius * 0.6f, 12, 12);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(0, 3, 10,    0, 1, 0,    0, 1, 0);
    
    drawSky();
    drawGround();
    
    // Update and draw dinosaurs
    for (auto& dino : dinosaurs) {
        dino.x += dino.speed;
        if (dino.x > 80.0f) dino.x = -80.0f;
        
        if (dino.type == 0) {
            drawTRex(dino.x, dino.z, dino.scale);
        } else {
            drawTriceratops(dino.x, dino.z, dino.scale);
        }
    }
    
    // Draw asteroid and blast
    drawAsteroid();
    drawBlast();
    
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
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(50, 50);
    
    // Change title based on asteroid activity
    const char* title;
    if (asteroidActive) {
        glColor3f(1.0f, 0.3f, 0.0f);
        title = "Cretaceous-Paleogene Extinction Event";
    } else {
        glColor3f(1.0f, 1.0f, 1.0f);
        title = "Chapter 14: Jurassic Period - Age of Dinosaurs";
    }
    
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
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
}

void timer(int v) {
    globalTime += 0.016f;
    
    // Asteroid appears after 8 seconds
    if (globalTime > 8.0f && !asteroidActive) {
        asteroidActive = true;
    }
    
    // Asteroid movement
    if (asteroidActive) {
        asteroidX -= 0.2f;
        asteroidY -= 0.15f;
        
        // Create blast when asteroid gets close to ground
        if (asteroidY < 2.0f) {
            blastRadius += 0.5f;
            blastIntensity = fmaxf(0.0f, 1.0f - (blastRadius / 50.0f));
            
            if (blastRadius > 50.0f) {
                // End scene after blast completes
                asteroidActive = false;
                blastRadius = 0.0f;
            }
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_position[] = {15.0f, 12.0f, 5.0f, 0.0f};
    GLfloat light_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat light_specular[] = {0.7f, 0.7f, 0.7f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void init() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);  // Gray background
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    setupLighting();
    initDinosaurs();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Chapter 14: Jurassic Period");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}
