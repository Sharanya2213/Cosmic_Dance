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
    // Create realistic vertical plants distributed across land in Z-depth
    for (int i = 0; i < 15; i++) {
        float plantX = 2.5f + (rand() % 70) / 10.0f;
        float plantZ = -4.5f + (rand() % 90) / 10.0f;  // Spread across Z depth
        plants.push_back({plantX, plantZ});
    }
}

// Draw a realistic vertical plant - early terrestrial bryophyte/early vascular plant
void drawPlant(float x, float z, float time) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    
    // Stem (vertical cylinder) - realistic plant stance
    GLUquadric* quad = gluNewQuadric();
    glColor4f(0.35f, 0.65f, 0.25f, 0.95f);
    
    // Subtle sway from wind (bending slightly)
    float sway = sinf(time * 1.0f + x * 0.15f + z * 0.1f) * 6.0f;
    glRotatef(sway, 0.0f, 0.0f, 1.0f);
    
    gluCylinder(quad, 0.12f, 0.10f, 1.1f, 8, 8);
    
    // Realistic fronds/leaves spiraling up the stem (primitive plant morphology)
    glDisable(GL_LIGHTING);
    glColor4f(0.25f, 0.75f, 0.2f, 0.88f);
    
    for (int i = 0; i < 8; i++) {
        glPushMatrix();
        // Position along stem height
        float heightPos = 0.15f + (i / 8.0f) * 0.85f;
        glTranslatef(0.0f, heightPos, 0.0f);
        
        // Spiral arrangement (alternating sides)
        float spiralAngle = (i / 8.0f) * 6.283f * 2.0f;
        float offsetX = 0.18f * cosf(spiralAngle);
        float offsetZ = 0.18f * sinf(spiralAngle);
        glTranslatef(offsetX, 0.0f, offsetZ);
        
        // Leaf frond (triangle pointing slightly upward)
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(offsetX * 0.8f, 0.25f, offsetZ * 0.8f);
        glVertex3f(offsetX * 0.6f, 0.35f, offsetZ * 0.6f);
        glEnd();
        
        glPopMatrix();
    }
    
    // Topmost frond cluster (apical meristem region)
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glColor4f(0.3f, 0.8f, 0.25f, 0.85f);
    for (int i = 0; i < 4; i++) {
        float angle = (i / 4.0f) * 6.283f;
        glPushMatrix();
        glTranslatef(0.15f * cosf(angle), 0.0f, 0.15f * sinf(angle));
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.1f * cosf(angle), 0.2f, 0.1f * sinf(angle));
        glVertex3f(0.08f * cosf(angle), 0.25f, 0.08f * sinf(angle));
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// Draw water plane with realistic wave physics
void drawOcean() {
    glColor4f(0.1f, 0.3f, 0.6f, 0.7f);
    glBegin(GL_QUADS);
    
    // Water surface with multi-frequency wave physics (realistic ocean waves)
    for (float x = -10.0f; x < 0.0f; x += 0.5f) {
        // Primary wave (main swell)
        float primaryWave1 = sinf((x + globalTime * 1.5f) * 0.8f) * 0.25f;
        float primaryWave2 = sinf((x + 0.5f + globalTime * 1.5f) * 0.8f) * 0.25f;
        float primaryWave3 = sinf((x + 0.5f + globalTime * 1.5f) * 0.8f) * 0.25f;
        float primaryWave4 = sinf((x + globalTime * 1.5f) * 0.8f) * 0.25f;
        
        // Secondary wave (choppy ripples)
        float secondaryWave1 = sinf((x + globalTime * 3.2f) * 2.5f) * 0.1f;
        float secondaryWave2 = sinf((x + 0.5f + globalTime * 3.2f) * 2.5f) * 0.1f;
        float secondaryWave3 = sinf((x + 0.5f + globalTime * 3.2f) * 2.5f) * 0.1f;
        float secondaryWave4 = sinf((x + globalTime * 3.2f) * 2.5f) * 0.1f;
        
        // Combined wave with depth variation
        float waveY1 = primaryWave1 + secondaryWave1 + cosf(x * 0.3f) * 0.05f;
        float waveY2 = primaryWave2 + secondaryWave2 + cosf((x + 0.5f) * 0.3f) * 0.05f;
        float waveY3 = primaryWave3 + secondaryWave3 + cosf((x + 0.5f) * 0.3f) * 0.05f;
        float waveY4 = primaryWave4 + secondaryWave4 + cosf(x * 0.3f) * 0.05f;
        
        glVertex3f(x, waveY1, -5.0f);
        glVertex3f(x + 0.5f, waveY2, -5.0f);
        glVertex3f(x + 0.5f, waveY3, 5.0f);
        glVertex3f(x, waveY4, 5.0f);
    }
    glEnd();
    
    // Ocean floor with depth gradient (darker way below)
    glColor4f(0.05f, 0.15f, 0.4f, 0.8f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, -2.5f, -5.0f);
    glVertex3f(0.0f, -2.5f, -5.0f);
    glVertex3f(0.0f, -2.5f, 5.0f);
    glVertex3f(-10.0f, -2.5f, 5.0f);
    glEnd();
    
    // Seabed details (sediment)
    glColor4f(0.08f, 0.12f, 0.25f, 0.9f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, -3.0f, -5.0f);
    glVertex3f(0.0f, -3.0f, -5.0f);
    glVertex3f(0.0f, -3.0f, 5.0f);
    glVertex3f(-10.0f, -3.0f, 5.0f);
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

// Draw fish with evolutionary transitional features
void drawFish(float x, float y, float z, bool isWalking) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // Apply gravity/buoyancy physics
    float gravityEffect = isWalking ? -0.15f : 0.0f;  // Gravity affects land walking
    
    // HEAD region with gills/lungs transition
    glPushMatrix();
    glTranslatef(0.4f, 0.0f, 0.0f);
    glColor4f(0.5f, 0.7f, 0.6f, 0.9f);
    glutSolidSphere(0.25f, 16, 16);
    
    // Gill slits (visible on swimming, fade on land)
    if (!isWalking) {
        glDisable(GL_LIGHTING);
        glColor4f(0.2f, 0.4f, 0.5f, 0.6f);
        // Gill lines
        for (int g = 0; g < 4; g++) {
            glPushMatrix();
            glTranslatef(-0.1f - g * 0.08f, -0.08f, 0.2f);
            glBegin(GL_LINES);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.15f, 0.0f);
            glEnd();
            glPopMatrix();
        }
        glEnable(GL_LIGHTING);
    }
    
    // Eye
    glPushMatrix();
    glTranslatef(0.15f, 0.1f, 0.15f);
    glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
    glutSolidSphere(0.06f, 8, 8);
    glPopMatrix();
    glPopMatrix();
    
    // BODY - more streamlined in water, stockier on land
    glPushMatrix();
    glTranslatef(0.0f, gravityEffect, 0.0f);
    float bodyScaleX = isWalking ? 1.4f : 2.5f;
    float bodyScaleY = isWalking ? 1.2f : 0.8f;
    float bodyScaleZ = isWalking ? 1.1f : 1.0f;
    glScalef(bodyScaleX, bodyScaleY, bodyScaleZ);
    glColor4f(0.4f, 0.7f, 0.6f, 0.85f);
    glutSolidSphere(0.4f, 20, 20);
    glPopMatrix();
    
    // Dorsal fin (large in water, reduced on land)
    if (!isWalking) {
        glPushMatrix();
        glTranslatef(0.0f, 0.35f, 0.0f);
        glColor4f(0.3f, 0.6f, 0.5f, 0.7f);
        float dorsal = sinf(globalTime * 3.0f) * 0.2f;
        glRotatef(dorsal * 10.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.15f, 0.4f, 0.08f);
        glutSolidSphere(0.4f, 10, 10);
        glPopMatrix();
    }
    
    // TAIL/POST-ANAL FIN region
    if (!isWalking) {
        glPushMatrix();
        glTranslatef(-0.5f, 0.0f, 0.0f);
        float tailWave = sinf(globalTime * 5.0f) * 0.3f;
        glRotatef(tailWave * 25.0f, 0.0f, 1.0f, 0.0f);
        glColor4f(0.35f, 0.65f, 0.55f, 0.8f);
        glutSolidCone(0.2f, 0.6f, 12, 12);
        glPopMatrix();
    } else {
        // Tail becomes caudal fin support for hind limbs
        glPushMatrix();
        glTranslatef(-0.5f, gravityEffect * 0.5f, 0.0f);
        glColor4f(0.35f, 0.65f, 0.55f, 0.5f);
        glutSolidSphere(0.12f, 8, 8);
        glPopMatrix();
    }
    
    // PECTORAL/PELVIC FINS to LIMBS transition with realistic tetrapod development
    if (!isWalking) {
        // Pectoral fins (evolving into front limbs)
        glColor4f(0.3f, 0.6f, 0.5f, 0.75f);
        GLUquadric* quad = gluNewQuadric();
        
        // Left pectoral fin
        glPushMatrix();
        glTranslatef(0.1f, -0.15f, 0.4f);
        float finWave1 = sinf(globalTime * 4.0f + 1.57f) * 0.4f;
        glRotatef(finWave1 * 30.0f, 0.0f, 0.0f, 1.0f);
        gluCylinder(quad, 0.08f, 0.06f, 0.5f, 8, 8);
        glPopMatrix();
        
        // Right pectoral fin
        glPushMatrix();
        glTranslatef(0.1f, -0.15f, -0.4f);
        float finWave2 = sinf(globalTime * 4.0f) * 0.4f;
        glRotatef(finWave2 * 30.0f, 0.0f, 0.0f, 1.0f);
        gluCylinder(quad, 0.08f, 0.06f, 0.5f, 8, 8);
        glPopMatrix();
        
        // Pelvic fins (smaller, evolving)
        glColor4f(0.25f, 0.55f, 0.45f, 0.6f);
        glPushMatrix();
        glTranslatef(-0.2f, -0.2f, 0.3f);
        gluCylinder(quad, 0.06f, 0.05f, 0.3f, 8, 8);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-0.2f, -0.2f, -0.3f);
        gluCylinder(quad, 0.06f, 0.05f, 0.3f, 8, 8);
        glPopMatrix();
    } else {
        // WALKING LEGS (tetrapod limbs on land) with gravity and biomechanics
        GLUquadric* quad = gluNewQuadric();
        glColor4f(0.2f, 0.5f, 0.4f, 0.95f);
        
        // Gravity affects leg position
        float legBaseY = -0.35f + gravityEffect * 0.5f;
        
        // Front left leg - pectoral evolved to forelimb (humerus, radius/ulna, digits)
        glPushMatrix();
        glTranslatef(0.25f, legBaseY, 0.35f);
        float legSwing1 = sinf(globalTime * 5.0f) * 50.0f;
        glRotatef(legSwing1, 1.0f, 0.0f, 0.0f);
        gluCylinder(quad, 0.13f, 0.11f, 0.65f, 10, 10);  // Humerus
        
        // Forearm/foot
        glTranslatef(0.0f, -0.65f, 0.0f);
        float forearmRot = sinf(globalTime * 5.0f) * 30.0f;
        glRotatef(forearmRot, 1.0f, 0.0f, 0.0f);
        glColor4f(0.22f, 0.52f, 0.42f, 0.9f);
        gluCylinder(quad, 0.11f, 0.09f, 0.4f, 10, 10);  // Radius/Ulna
        glTranslatef(0.0f, -0.4f, 0.0f);
        glColor4f(0.18f, 0.48f, 0.38f, 0.85f);
        glutSolidSphere(0.16f, 10, 10);  // Carpus/digits
        glPopMatrix();
        
        // Front right leg - opposite phase
        glPushMatrix();
        glTranslatef(0.25f, legBaseY, -0.35f);
        float legSwing2 = sinf(globalTime * 5.0f + 3.14159f) * 50.0f;
        glRotatef(legSwing2, 1.0f, 0.0f, 0.0f);
        glColor4f(0.2f, 0.5f, 0.4f, 0.95f);
        gluCylinder(quad, 0.13f, 0.11f, 0.65f, 10, 10);
        glTranslatef(0.0f, -0.65f, 0.0f);
        float forearmRot2 = sinf(globalTime * 5.0f + 3.14159f) * 30.0f;
        glRotatef(forearmRot2, 1.0f, 0.0f, 0.0f);
        glColor4f(0.22f, 0.52f, 0.42f, 0.9f);
        gluCylinder(quad, 0.11f, 0.09f, 0.4f, 10, 10);
        glTranslatef(0.0f, -0.4f, 0.0f);
        glColor4f(0.18f, 0.48f, 0.38f, 0.85f);
        glutSolidSphere(0.16f, 10, 10);
        glPopMatrix();
        
        // Rear left leg - pelvic evolved to hind limb (femur, tibia/fibula, tarsus)
        glPushMatrix();
        glTranslatef(-0.25f, legBaseY + 0.05f, 0.35f);
        float legSwing3 = sinf(globalTime * 5.0f + 3.14159f) * 50.0f;
        glRotatef(legSwing3, 1.0f, 0.0f, 0.0f);
        glColor4f(0.25f, 0.55f, 0.45f, 0.95f);
        gluCylinder(quad, 0.14f, 0.12f, 0.7f, 10, 10);  // Femur (stronger)
        glTranslatef(0.0f, -0.7f, 0.0f);
        float tibiRot = sinf(globalTime * 5.0f + 3.14159f) * 40.0f;
        glRotatef(tibiRot, 1.0f, 0.0f, 0.0f);
        glColor4f(0.27f, 0.57f, 0.47f, 0.9f);
        gluCylinder(quad, 0.12f, 0.10f, 0.5f, 10, 10);  // Tibia/Fibula
        glTranslatef(0.0f, -0.5f, 0.0f);
        glColor4f(0.23f, 0.53f, 0.43f, 0.85f);
        glutSolidSphere(0.17f, 10, 10);  // Tarsus/digits
        glPopMatrix();
        
        // Rear right leg - opposite phase
        glPushMatrix();
        glTranslatef(-0.25f, legBaseY + 0.05f, -0.35f);
        float legSwing4 = sinf(globalTime * 5.0f) * 50.0f;
        glRotatef(legSwing4, 1.0f, 0.0f, 0.0f);
        glColor4f(0.25f, 0.55f, 0.45f, 0.95f);
        gluCylinder(quad, 0.14f, 0.12f, 0.7f, 10, 10);
        glTranslatef(0.0f, -0.7f, 0.0f);
        float tibiRot2 = sinf(globalTime * 5.0f) * 40.0f;
        glRotatef(tibiRot2, 1.0f, 0.0f, 0.0f);
        glColor4f(0.27f, 0.57f, 0.47f, 0.9f);
        gluCylinder(quad, 0.12f, 0.10f, 0.5f, 10, 10);
        glTranslatef(0.0f, -0.5f, 0.0f);
        glColor4f(0.23f, 0.53f, 0.43f, 0.85f);
        glutSolidSphere(0.17f, 10, 10);
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
    const char* status = isWalking ? "Status: TERRESTRIAL TETRAPOD - Advanced limb development" : "Status: AQUATIC - Fin-based locomotion";
    for (const char* c = status; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    
    glRasterPos2f(50, 110);
    const char* evolution = isWalking ? "Evolutionary Stage: Late Devonian tetrapod (Humerus-Radius/Ulna-Carpus-Digits)" : "Evolutionary Stage: Fish with developing limbs";
    for (const char* c = evolution; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    
    glRasterPos2f(50, 135);
    const char* physics = isWalking ? "Physics: Gravity, terrestrial locomotion, joint-based biomechanics" : "Physics: Buoyancy, drag coefficient, fin wave propagation";
    for (const char* c = physics; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
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
