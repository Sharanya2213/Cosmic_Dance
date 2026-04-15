#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdio>

// ============================================================================
// GLOBALS
// ============================================================================

float globalTime = 0.0f;

struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float life, age;
};

struct Tree {
    float x, z;
    float height;
};

std::vector<Particle> smoke;
std::vector<Tree> trees;

// ============================================================================
// INITIALIZATION
// ============================================================================

void initTrees() {
    trees.clear();
    srand(12345);  // Fixed seed for consistent placement
    
    // Generate 50 trees randomly on island (distance < 9)
    for (int i = 0; i < 50; i++) {
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float dist = (rand() % 90) / 10.0f;  // 0 to 9
        
        Tree t;
        t.x = cosf(angle) * dist;
        t.z = sinf(angle) * dist;
        t.height = 1.5f + (rand() % 30) / 100.0f;  // 1.5 to 1.8
        
        if (t.x*t.x + t.z*t.z < 81.0f) {  // Ensure inside island
            trees.push_back(t);
        }
    }
    
    printf("Initialized %zu trees on island\n", trees.size());
}

// ============================================================================
// RENDERING FUNCTIONS
// ============================================================================

void drawOcean() {
    // Large ocean plane with wave animation: X: -30 to +30, Z: -30 to +30
    glColor4f(0.0f, 0.3f, 0.6f, 0.9f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    float step = 2.0f;
    for (float x = -30.0f; x < 30.0f; x += step) {
        glBegin(GL_QUAD_STRIP);
        for (float z = -30.0f; z <= 30.0f; z += step) {
            // Ocean wave animation: y = sin(x * 0.3 + globalTime) * 0.1
            float wave = sinf(x * 0.3f + globalTime) * 0.1f;
            glVertex3f(x, wave - 0.5f, z);
            glVertex3f(x + step, wave - 0.5f, z);
        }
        glEnd();
    }
    glDisable(GL_BLEND);
}

void drawIsland() {
    // Circular island at center: distance < 10 is land
    glColor3f(0.2f, 0.6f, 0.2f);
    
    float step = 0.8f;
    for (float x = -10.0f; x < 10.0f; x += step) {
        glBegin(GL_QUAD_STRIP);
        for (float z = -10.0f; z <= 10.0f; z += step) {
            float dist = sqrtf(x*x + z*z);
            
            // Only draw inside circle (distance < 10)
            if (dist < 10.0f) {
                // Slight elevation: y = 0.2 + sin(x*0.3 + z*0.3)*0.1
                float y = 0.2f + sinf(x * 0.3f + z * 0.3f) * 0.1f;
                glVertex3f(x, y, z);
                glVertex3f(x + step, y, z);
            }
        }
        glEnd();
    }
}

void drawVolcano() {
    // ONE large volcano at center (0, 0)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    
    // Main cone: base ~3, height ~6 (NO rotation - must be vertical!)
    glColor3f(0.5f, 0.4f, 0.3f);
    glutSolidCone(3.0f, 6.0f, 20, 16);
    
    // Crater glow at top
    glPushMatrix();
    glTranslatef(0.0f, 6.0f, 0.0f);
    float glow = 0.4f + sinf(globalTime * 3.0f) * 0.3f;
    glColor4f(1.0f, 0.5f, 0.2f, glow);
    glEnable(GL_BLEND);
    glutSolidSphere(1.5f, 12, 10);
    glDisable(GL_BLEND);
    glPopMatrix();
    
    glPopMatrix();
    
    // Spawn smoke particles from crater (0, 6, 0)
    if (rand() % 5 == 0) {
        for (int i = 0; i < 2; i++) {
            Particle p;
            p.x = 0.0f + (rand() % 30 - 15) / 50.0f;
            p.y = 6.0f;
            p.z = 0.0f + (rand() % 30 - 15) / 50.0f;
            p.vx = (rand() % 10 - 5) / 200.0f;
            p.vy = 0.05f + (rand() % 10) / 200.0f;  // Move upward
            p.vz = (rand() % 10 - 5) / 200.0f;
            p.life = 1.0f;
            p.age = 0.0f;
            smoke.push_back(p);
        }
    }
}

void drawSmoke() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    auto it = smoke.begin();
    while (it != smoke.end()) {
        it->age += 0.016f;
        
        // Move upward ONLY
        it->x += it->vx;
        it->y += it->vy;
        it->z += it->vz;
        
        if (it->age >= it->life) {
            it = smoke.erase(it);
        } else {
            float alpha = 0.5f * (1.0f - it->age / it->life);
            glColor4f(0.7f, 0.7f, 0.7f, alpha);
            
            glPushMatrix();
            glTranslatef(it->x, it->y, it->z);
            float size = 0.2f + (it->age / it->life) * 0.5f;
            glScalef(size, size, size);
            glutSolidSphere(1.0f, 6, 5);
            glPopMatrix();
            
            ++it;
        }
    }
    glDisable(GL_BLEND);
}

void drawTree(const Tree& t) {
    // Trees only on island: inside radius, slightly elevated
    float ty = 0.2f + sinf(t.x * 0.3f + t.z * 0.3f) * 0.1f;
    
    glPushMatrix();
    glTranslatef(t.x, ty, t.z);
    
    // Trunk: vertical cylinder
    glColor3f(0.4f, 0.25f, 0.1f);
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  // CRITICAL: rotate around X-axis only
    gluCylinder(gluNewQuadric(), 0.08f, 0.06f, t.height, 8, 6);
    glPopMatrix();
    
    // Foliage: cone pointing upward (NO rotation)
    glColor3f(0.3f, 0.55f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, t.height, 0.0f);
    glutSolidCone(0.4f, 0.8f, 10, 8);
    glPopMatrix();
    
    glPopMatrix();
}

void drawAllTrees() {
    for (const auto& t : trees) {
        drawTree(t);
    }
}

// ============================================================================
// DISPLAY & CALLBACKS
// ============================================================================

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Static camera (FIXED) at (18, 12, 18)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(18, 12, 18,    0, 0, 0,    0, 1, 0);
    
    // Draw scene
    drawOcean();
    drawIsland();
    drawVolcano();
    drawSmoke();
    drawAllTrees();
    
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
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// ============================================================================
// SETUP & MAIN
// ============================================================================

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_NORMALIZE);
    
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    
    // Simple lighting from above
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_pos[] = {10.0f, 15.0f, 10.0f, 0.0f};
    GLfloat light_amb[] = {0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat light_dif[] = {0.7f, 0.7f, 0.7f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    
    initTrees();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Island with Volcano and Dense Forest");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    
    return 0;
}
