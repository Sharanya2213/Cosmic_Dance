#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

struct vec3 {
    float x, y, z;
    vec3() : x(0), y(0), z(0) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    vec3 operator+(const vec3& v) const { return vec3(x+v.x, y+v.y, z+v.z); }
    vec3 operator-(const vec3& v) const { return vec3(x-v.x, y-v.y, z-v.z); }
    float length() const { return sqrtf(x*x + y*y + z*z); }
};

struct Atom {
    vec3 pos;
    vec3 basePos;
    float offset;
    int type;
    float brightness;
};

struct Microorganism {
    vec3 centerPos;
    vec3 baseCenterPos;
    float centerOffset;
    std::vector<vec3> attachedSpheres;
    std::vector<vec3> baseAttached;
    float rotationX, rotationY, rotationZ;
    float rotSpeed;
    float colorPhase;
};

const int NUM_ATOMS = 250;
const int NUM_MICROORGANISMS = 25;
const float BOND_DISTANCE = 0.5f;

Atom atoms[NUM_ATOMS];
std::vector<Microorganism> microorganisms;

float globalTime = 0.0f;
int windowWidth = 1400;
int windowHeight = 900;

void initAtoms() {
    for (int i = 0; i < NUM_ATOMS; ++i) {
        atoms[i].basePos = vec3(
            (rand() % 600 - 300) * 0.012f,
            (rand() % 600 - 300) * 0.012f,
            (rand() % 600 - 300) * 0.012f
        );
        atoms[i].pos = atoms[i].basePos;
        atoms[i].offset = (rand() % 1000) * 0.001f;
        atoms[i].type = rand() % 4;
        atoms[i].brightness = 0.5f + (rand() % 50) * 0.01f;
    }
}

void initMicroorganisms() {
    for (int i = 0; i < NUM_MICROORGANISMS; ++i) {
        Microorganism org;
        
        org.baseCenterPos = vec3(
            (rand() % 500 - 250) * 0.008f,
            (rand() % 500 - 250) * 0.008f,
            (rand() % 500 - 250) * 0.008f - 1.0f
        );
        org.centerPos = org.baseCenterPos;
        org.centerOffset = (rand() % 1000) * 0.001f;
        org.colorPhase = (rand() % 100) * 0.01f;
        
        int numAttached = 4 + rand() % 4;
        for (int j = 0; j < numAttached; ++j) {
            float angle = (j * 360.0f / numAttached) + (rand() % 50 - 25) * 0.5f;
            float dist = 0.18f + (rand() % 30) * 0.01f;
            vec3 attachPos(
                dist * cosf(angle * 3.14159f / 180.0f),
                (rand() % 30 - 15) * 0.01f,
                dist * sinf(angle * 3.14159f / 180.0f)
            );
            org.baseAttached.push_back(attachPos);
            org.attachedSpheres.push_back(attachPos);
        }
        
        org.rotationX = 0.0f;
        org.rotationY = 0.0f;
        org.rotationZ = 0.0f;
        org.rotSpeed = 0.8f + (rand() % 40) * 0.01f;
        
        microorganisms.push_back(org);
    }
}

void updateAtoms() {
    for (int i = 0; i < NUM_ATOMS; ++i) {
        float t = globalTime + atoms[i].offset;
        atoms[i].pos.x = atoms[i].basePos.x + sinf(t * 1.2f) * 0.002f;
        atoms[i].pos.y = atoms[i].basePos.y + cosf(t * 0.9f) * 0.002f;
        atoms[i].pos.z = atoms[i].basePos.z + sinf(t * 0.5f) * 0.002f;
    }
}

void updateMicroorganisms() {
    for (auto& org : microorganisms) {
        float t = globalTime + org.centerOffset;
        
        org.centerPos.x = org.baseCenterPos.x + sinf(t * 0.4f) * 0.004f;
        org.centerPos.y = org.baseCenterPos.y + cosf(t * 0.3f) * 0.004f;
        org.centerPos.z = org.baseCenterPos.z + sinf(t * 0.25f) * 0.003f;
        
        org.rotationX += org.rotSpeed * 0.8f;
        org.rotationY += org.rotSpeed * 1.1f;
        org.rotationZ += org.rotSpeed * 0.4f;
        
        for (size_t i = 0; i < org.attachedSpheres.size(); ++i) {
            org.attachedSpheres[i] = org.baseAttached[i];
        }
    }
}

void drawAtom(const vec3& pos, int type) {
    float r, g, b;
    
    switch(type) {
        case 0: r = 0.1f; g = 0.7f; b = 1.0f; break;   // Cyan
        case 1: r = 1.0f; g = 0.2f; b = 0.5f; break;   // Magenta
        case 2: r = 0.1f; g = 1.0f; b = 0.3f; break;   // Bright Green
        default: r = 1.0f; g = 0.8f; b = 0.1f; break;  // Yellow
    }
    
    glColor4f(r, g, b, 0.9f);
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glutSolidSphere(0.06f, 12, 12);
    glPopMatrix();
    
    // Glow effect
    glDisable(GL_LIGHTING);
    glColor4f(r, g, b, 0.3f);
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glutSolidSphere(0.15f, 8, 8);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawBonds() {
    glColor4f(0.6f, 0.8f, 1.0f, 0.5f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    
    for (int i = 0; i < NUM_ATOMS; ++i) {
        for (int j = i + 1; j < NUM_ATOMS; ++j) {
            float dist = (atoms[i].pos - atoms[j].pos).length();
            
            if (dist < BOND_DISTANCE && globalTime > 1.5f) {
                float bondAlpha = (globalTime - 1.5f) / 4.0f;
                if (bondAlpha > 1.0f) bondAlpha = 1.0f;
                
                glColor4f(0.5f, 0.9f, 1.0f, bondAlpha * 0.6f);
                glVertex3f(atoms[i].pos.x, atoms[i].pos.y, atoms[i].pos.z);
                glVertex3f(atoms[j].pos.x, atoms[j].pos.y, atoms[j].pos.z);
            }
        }
    }
    glEnd();
    glLineWidth(1.0f);
}

void drawMicroorganism(const Microorganism& org) {
    float microAlpha = (globalTime - 4.5f) / 2.5f;
    if (microAlpha < 0.0f) return;
    if (microAlpha > 1.0f) microAlpha = 1.0f;
    
    glPushMatrix();
    glTranslatef(org.centerPos.x, org.centerPos.y, org.centerPos.z);
    glRotatef(org.rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(org.rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(org.rotationZ, 0.0f, 0.0f, 1.0f);
    
    // Pulsing core with color shift
    float pulse = 1.0f + sinf(globalTime * 2.5f) * 0.15f;
    float colorCycle = sinf(globalTime * 0.8f + org.colorPhase) * 0.5f + 0.5f;
    
    float coreR = 0.2f + colorCycle * 0.4f;
    float coreG = 0.6f + colorCycle * 0.4f;
    float coreB = 0.9f - colorCycle * 0.3f;
    
    glColor4f(coreR, coreG, coreB, 0.8f * microAlpha);
    glPushMatrix();
    glScalef(pulse, pulse, pulse);
    glutSolidSphere(0.22f, 14, 14);
    glPopMatrix();
    
    // Glow halo
    glDisable(GL_LIGHTING);
    glColor4f(coreR, coreG, coreB, 0.25f * microAlpha);
    glPushMatrix();
    glScalef(pulse * 1.8f, pulse * 1.8f, pulse * 1.8f);
    glutSolidSphere(0.22f, 10, 10);
    glPopMatrix();
    glEnable(GL_LIGHTING);
    
    // Attached spheres with animation
    for (size_t i = 0; i < org.attachedSpheres.size(); ++i) {
        const vec3& attachPos = org.attachedSpheres[i];
        
        float attachR = 0.3f + cosf(globalTime + i * 0.5f) * 0.3f;
        float attachG = 0.7f + sinf(globalTime * 1.3f + i) * 0.3f;
        float attachB = 0.9f + cosf(globalTime * 0.7f - i) * 0.2f;
        
        glColor4f(attachR, attachG, attachB, 0.7f * microAlpha);
        glPushMatrix();
        glTranslatef(attachPos.x, attachPos.y, attachPos.z);
        
        float attachScale = pulse * (0.85f + 0.15f * sinf(globalTime * 2.0f + i));
        glScalef(attachScale, attachScale, attachScale);
        glutSolidSphere(0.14f, 12, 12);
        glPopMatrix();
        
        // Glow for attached spheres
        glDisable(GL_LIGHTING);
        glColor4f(attachR, attachG, attachB, 0.2f * microAlpha);
        glPushMatrix();
        glTranslatef(attachPos.x, attachPos.y, attachPos.z);
        glScalef(attachScale * 1.6f, attachScale * 1.6f, attachScale * 1.6f);
        glutSolidSphere(0.14f, 8, 8);
        glPopMatrix();
        glEnable(GL_LIGHTING);
    }
    
    glPopMatrix();
}

void drawStarfield() {
    glDisable(GL_LIGHTING);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    
    for (int i = 0; i < 3000; ++i) {
        float x = -40.0f + (rand() % 8000) / 100.0f;
        float y = -30.0f + (rand() % 6000) / 100.0f;
        float z = -40.0f + (rand() % 8000) / 100.0f;
        
        float brightness = 0.4f + (rand() % 60) / 100.0f;
        glColor3f(brightness, brightness * 0.9f, brightness + 0.1f);
        glVertex3f(x, y, z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Dynamic cinematic camera
    float camDist = 10.0f + 2.0f * sinf(globalTime * 0.3f);
    float camAngle = globalTime * 0.15f;
    float camHeight = 1.0f + 1.5f * cosf(globalTime * 0.25f);
    
    gluLookAt(camDist * cosf(camAngle), camHeight, camDist * sinf(camAngle),
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
    
    drawStarfield();
    
    // Draw atoms
    for (int i = 0; i < NUM_ATOMS; ++i) {
        drawAtom(atoms[i].pos, atoms[i].type);
    }
    
    // Draw bonds
    drawBonds();
    
    // Draw microorganisms
    for (const auto& org : microorganisms) {
        drawMicroorganism(org);
    }

    // Draw Chapter Title in top-left corner
    float titleAlpha = 1.0f;
    if (globalTime < 0.5f) {
        titleAlpha = globalTime / 0.5f;
    } else if (globalTime > 24.5f) {
        titleAlpha = (25.0f - globalTime) / 0.5f;
    }
    
    if (titleAlpha > 0.0f) {
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glColor4f(1.0f, 1.0f, 1.0f, titleAlpha);
        glRasterPos2f(-0.95f, 0.90f);
        const char* titleText = "Chapter 7: Precambrian Era - Emergence of Life";
        for (const char* c = titleText; *c; c++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
    }

    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)w / (float)h;
    gluPerspective(65.0f, aspect, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
}

void idle() {
    globalTime += 0.016f;
    
    if (globalTime > 18.0f) {
        exit(0);
    }
    
    updateAtoms();
    updateMicroorganisms();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    if (key == ' ') globalTime = 0.0f;
}

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glClearColor(0.01f, 0.02f, 0.04f, 1.0f);
    
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_pos[] = {3.0f, 3.0f, 3.0f, 0.0f};
    GLfloat light_amb[] = {0.3f, 0.3f, 0.4f, 1.0f};
    GLfloat light_dif[] = {0.8f, 0.8f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    
    initAtoms();
    initMicroorganisms();
}

int main(int argc, char** argv) {
    srand((unsigned int)time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Emergence of Life - Cosmic Dance");
    
    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}
