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

// Draw a realistic vertical plant - Devonian early terrestrial plant
void drawPlant(float x, float z, float time) {
    glPushMatrix();
    
    // Position tree on ground at base
    glTranslatef(x, 0.0f, z);
    
    // ===== TRUNK =====
    // Cylinder is along Z by default → rotate -90° around X axis to make it grow along Y
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    
    glColor3f(0.4f, 0.25f, 0.1f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.08f, 0.06f, 1.2f, 10, 10);
    
    glPopMatrix();
    
    // ===== FOLIAGE/LEAVES =====
    glPushMatrix();
    
    // Move to top of trunk
    glTranslatef(x, 1.2f, z);
    
    // Slight sway effect (gentle wind)
    float sway = sinf(time * 0.8f + x * 0.1f + z * 0.1f) * 4.0f;
    glRotatef(sway, 0.0f, 0.0f, 1.0f);
    
    // Main foliage crown (cone shape)
    glColor3f(0.15f, 0.65f, 0.2f);
    glutSolidCone(0.35f, 0.8f, 16, 16);
    
    // Secondary foliage layer (sphere for fullness)
    glPushMatrix();
    glTranslatef(0.0f, 0.25f, 0.0f);
    glColor3f(0.2f, 0.7f, 0.25f);
    glutSolidSphere(0.3f, 14, 14);
    glPopMatrix();
    
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

// Draw tetrapod creature - different morphology for water vs land
void drawFish(float x, float y, float z, bool isWalking) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    float gravityEffect = isWalking ? -0.15f : 0.0f;
    
    if (!isWalking) {
        // ===== AQUATIC FORM =====
        // More fish-like with prominent fins
        
        // HEAD - smaller, streamlined
        glPushMatrix();
        glTranslatef(0.35f, 0.0f, 0.0f);
        glColor3f(0.70f, 0.65f, 0.35f);
        glScalef(1.1f, 0.9f, 1.0f);
        glutSolidSphere(0.22f, 16, 16);
        glPopMatrix();
        
        // Small snout
        glPushMatrix();
        glTranslatef(0.55f, 0.0f, 0.0f);
        glColor3f(0.68f, 0.63f, 0.33f);
        glScalef(0.7f, 0.8f, 0.85f);
        glutSolidSphere(0.12f, 12, 12);
        glPopMatrix();
        
        // Eye
        glPushMatrix();
        glTranslatef(0.30f, 0.12f, 0.18f);
        glColor3f(0.0f, 0.0f, 0.0f);
        glutSolidSphere(0.07f, 8, 8);
        glPopMatrix();
        
        // ELONGATED BODY - streamlined for water
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        glColor3f(0.65f, 0.60f, 0.30f);
        glScalef(2.8f, 0.85f, 0.88f);
        glutSolidSphere(0.35f, 20, 20);
        glPopMatrix();
        
        // LARGE DORSAL FIN (prominent in water)
        glPushMatrix();
        glTranslatef(-0.15f, 0.32f, 0.0f);
        glColor3f(0.55f, 0.50f, 0.25f);
        float dorsal = sinf(globalTime * 3.0f) * 0.15f;
        glRotatef(dorsal * 5.0f, 1.0f, 0.0f, 0.0f);
        
        // Dorsal fin ridges
        for (int d = 0; d < 6; d++) {
            float finX = -0.2f + (d * 0.12f);
            float finHeight = 0.40f - (fabs(d - 2.5f) * 0.07f);
            glPushMatrix();
            glTranslatef(finX, 0.0f, 0.0f);
            glScalef(0.08f, finHeight, 0.15f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
        glPopMatrix();
        
        // LARGE TAIL FIN (for aquatic propulsion)
        glPushMatrix();
        glTranslatef(-0.65f, 0.0f, 0.0f);
        float tailWave = sinf(globalTime * 4.5f) * 0.4f;
        glRotatef(tailWave * 25.0f, 0.0f, 1.0f, 0.0f);
        glColor3f(0.60f, 0.55f, 0.28f);
        glutSolidCone(0.22f, 0.65f, 16, 16);
        glPopMatrix();
        
        // PECTORAL FINS (large, fin-like)
        glColor3f(0.62f, 0.57f, 0.32f);
        
        // Left pectoral fin
        glPushMatrix();
        glTranslatef(0.1f, -0.1f, 0.45f);
        float finWave1 = sinf(globalTime * 3.5f) * 0.35f;
        glRotatef(finWave1 * 35.0f, 0.0f, 0.0f, 1.0f);
        glScalef(1.0f, 0.15f, 0.6f);
        glutSolidSphere(0.18f, 12, 12);
        glPopMatrix();
        
        // Right pectoral fin
        glPushMatrix();
        glTranslatef(0.1f, -0.1f, -0.45f);
        float finWave2 = sinf(globalTime * 3.5f) * 0.35f;
        glRotatef(finWave2 * 35.0f, 0.0f, 0.0f, 1.0f);
        glScalef(1.0f, 0.15f, 0.6f);
        glutSolidSphere(0.18f, 12, 12);
        glPopMatrix();
        
        // PELVIC FINS (smaller, visible)
        glColor3f(0.58f, 0.53f, 0.28f);
        glPushMatrix();
        glTranslatef(-0.15f, -0.25f, 0.32f);
        glScalef(0.65f, 0.12f, 0.35f);
        glutSolidSphere(0.12f, 10, 10);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-0.15f, -0.25f, -0.32f);
        glScalef(0.65f, 0.12f, 0.35f);
        glutSolidSphere(0.12f, 10, 10);
        glPopMatrix();
        // SCALES (visual detail)
        glDisable(GL_LIGHTING);
        glColor3f(0.55f, 0.50f, 0.25f);
        for (int s = 0; s < 5; s++) {
            float scaleX = -0.2f + (s * 0.25f);
            for (int sz = 0; sz < 3; sz++) {
                float offset = (sz - 1) * 0.2f;
                glPushMatrix();
                glTranslatef(scaleX, 0.15f, offset);
                glBegin(GL_LINES);
                glVertex3f(-0.08f, 0.0f, 0.0f);
                glVertex3f(0.08f, 0.0f, 0.0f);
                glEnd();
                glPopMatrix();
            }
        }
        glEnable(GL_LIGHTING);
        
    } else {
        // ===== TERRESTRIAL FORM =====
        // Robust tetrapod with four strong limbs
        
        // Large prominent head
        glPushMatrix();
        glTranslatef(0.45f, 0.05f, 0.0f);
        glColor3f(0.45f, 0.65f, 0.50f);
        glScalef(1.3f, 1.0f, 1.1f);
        glutSolidSphere(0.28f, 18, 18);
        glPopMatrix();
        
        // Snout/jaw (protruding)
        glPushMatrix();
        glTranslatef(0.7f, 0.05f, 0.0f);
        glColor3f(0.40f, 0.60f, 0.45f);
        glScalef(0.8f, 0.65f, 0.9f);
        glutSolidSphere(0.18f, 14, 14);
        glPopMatrix();
        
        // TEETH on lower jaw
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 0.95f, 0.85f);
        for (int t = 0; t < 8; t++) {
            glPushMatrix();
            glTranslatef(0.62f + (t * 0.08f), -0.12f, -0.12f);
            glScalef(0.08f, 0.15f, 0.06f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
        glEnable(GL_LIGHTING);
        
        // Eye
        glPushMatrix();
        glTranslatef(0.35f, 0.15f, 0.2f);
        glColor3f(0.0f, 0.0f, 0.0f);
        glutSolidSphere(0.08f, 10, 10);
        glPopMatrix();
        
        // ROBUST BODY
        glPushMatrix();
        glTranslatef(0.0f, gravityEffect, 0.0f);
        glColor3f(0.50f, 0.70f, 0.55f);
        glScalef(2.0f, 1.1f, 0.95f);
        glutSolidSphere(0.38f, 20, 20);
        glPopMatrix();
        
        // Visible vertebral column
        glDisable(GL_LIGHTING);
        glColor3f(0.35f, 0.55f, 0.40f);
        for (int r = 0; r < 6; r++) {
            float ribX = -0.1f + (r * 0.25f);
            for (int s = 0; s < 3; s++) {
                float offset = (s - 1) * 0.15f;
                glPushMatrix();
                glTranslatef(ribX, 0.25f + gravityEffect, offset);
                glBegin(GL_LINES);
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, -0.35f, 0.0f);
                glEnd();
                glPopMatrix();
            }
        }
        glEnable(GL_LIGHTING);
        
        // ===== FOUR LIMBS =====
        GLUquadric* quad = gluNewQuadric();
        
        // FRONT LEFT LIMB
        glPushMatrix();
        glTranslatef(0.2f, -0.25f + gravityEffect, 0.38f);
        float frontLeftSwing = sinf(globalTime * 5.0f) * 45.0f;
        glRotatef(frontLeftSwing, 1.0f, 0.0f, 0.0f);
        glColor3f(0.48f, 0.68f, 0.58f);
        gluCylinder(quad, 0.14f, 0.12f, 0.7f, 12, 12);
        glTranslatef(0.0f, -0.7f, 0.0f);
        float forearmSwing = sinf(globalTime * 5.0f) * 30.0f;
        glRotatef(forearmSwing, 1.0f, 0.0f, 0.0f);
        glColor3f(0.45f, 0.65f, 0.55f);
        gluCylinder(quad, 0.12f, 0.10f, 0.5f, 12, 12);
        glTranslatef(0.0f, -0.5f, 0.0f);
        glColor3f(0.42f, 0.62f, 0.52f);
        glutSolidSphere(0.12f, 10, 10);
        glPopMatrix();
        
        // FRONT RIGHT LIMB
        glPushMatrix();
        glTranslatef(0.2f, -0.25f + gravityEffect, -0.38f);
        float frontRightSwing = sinf(globalTime * 5.0f + 3.14159f) * 45.0f;
        glRotatef(frontRightSwing, 1.0f, 0.0f, 0.0f);
        glColor3f(0.48f, 0.68f, 0.58f);
        gluCylinder(quad, 0.14f, 0.12f, 0.7f, 12, 12);
        glTranslatef(0.0f, -0.7f, 0.0f);
        float forearmSwing2 = sinf(globalTime * 5.0f + 3.14159f) * 30.0f;
        glRotatef(forearmSwing2, 1.0f, 0.0f, 0.0f);
        glColor3f(0.45f, 0.65f, 0.55f);
        gluCylinder(quad, 0.12f, 0.10f, 0.5f, 12, 12);
        glTranslatef(0.0f, -0.5f, 0.0f);
        glColor3f(0.42f, 0.62f, 0.52f);
        glutSolidSphere(0.12f, 10, 10);
        glPopMatrix();
        
        // HIND LEFT LIMB
        glPushMatrix();
        glTranslatef(-0.2f, -0.30f + gravityEffect, 0.36f);
        float hindLeftSwing = sinf(globalTime * 5.0f + 3.14159f) * 50.0f;
        glRotatef(hindLeftSwing, 1.0f, 0.0f, 0.0f);
        glColor3f(0.52f, 0.72f, 0.62f);
        gluCylinder(quad, 0.16f, 0.14f, 0.8f, 12, 12);
        glTranslatef(0.0f, -0.8f, 0.0f);
        float shinSwing = sinf(globalTime * 5.0f + 3.14159f) * 35.0f;
        glRotatef(shinSwing, 1.0f, 0.0f, 0.0f);
        glColor3f(0.50f, 0.70f, 0.60f);
        gluCylinder(quad, 0.13f, 0.11f, 0.6f, 12, 12);
        glTranslatef(0.0f, -0.6f, 0.0f);
        glColor3f(0.48f, 0.68f, 0.58f);
        glutSolidSphere(0.13f, 10, 10);
        glPopMatrix();
        
        // HIND RIGHT LIMB
        glPushMatrix();
        glTranslatef(-0.2f, -0.30f + gravityEffect, -0.36f);
        float hindRightSwing = sinf(globalTime * 5.0f) * 50.0f;
        glRotatef(hindRightSwing, 1.0f, 0.0f, 0.0f);
        glColor3f(0.52f, 0.72f, 0.62f);
        gluCylinder(quad, 0.16f, 0.14f, 0.8f, 12, 12);
        glTranslatef(0.0f, -0.8f, 0.0f);
        float shinSwing2 = sinf(globalTime * 5.0f) * 35.0f;
        glRotatef(shinSwing2, 1.0f, 0.0f, 0.0f);
        glColor3f(0.50f, 0.70f, 0.60f);
        gluCylinder(quad, 0.13f, 0.11f, 0.6f, 12, 12);
        glTranslatef(0.0f, -0.6f, 0.0f);
        glColor3f(0.48f, 0.68f, 0.58f);
        glutSolidSphere(0.13f, 10, 10);
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
    const char* status = isWalking ? "Status: TERRESTRIAL TETRAPOD - Limb-based locomotion" : "Status: AQUATIC - Fin-based locomotion";
    for (const char* c = status; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    
    glRasterPos2f(50, 110);
    const char* evolution = isWalking ? "Evolutionary Stage: Late Devonian tetrapod" : "Evolutionary Stage: Early fish with developing limbs";
    for (const char* c = evolution; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    
    glRasterPos2f(50, 135);
    const char* adaptation = isWalking ? "Adaptation: Terrestrial colonization" : "Adaptation: Aquatic environment mastery";
    for (const char* c = adaptation; *c; c++) {
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
