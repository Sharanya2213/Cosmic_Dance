#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

// Window dimensions
int windowWidth = 1200;
int windowHeight = 800;

// Global time
float globalTime = 0.0f;

// Camera parameters
float cameraDistance = 20.0f;
float cameraAngle = 0.0f;

// Particle structure
struct Particle {
    float x, y, z;           // Position
    float r, g, b;           // Color
    float alpha;             // Transparency
    float originalX, originalY, originalZ;  // Original position
};

// Star structure
struct Star {
    float x, y, z;
    float brightness;
};

const int NUM_NEBULA_PARTICLES = 8000;
const int NUM_STARS = 2000;

Particle nebulaPart[NUM_NEBULA_PARTICLES];
Star stars[NUM_STARS];

// Initialize starfield
void initStars() {
    for (int i = 0; i < NUM_STARS; ++i) {
        stars[i].x = (rand() % 2000 - 1000) * 0.1f;
        stars[i].y = (rand() % 2000 - 1000) * 0.1f;
        stars[i].z = (rand() % 2000 - 1000) * 0.1f;
        stars[i].brightness = 0.3f + (rand() % 100) / 200.0f;
    }
}

// Initialize nebula particles
void initNebula() {
    // Color palette
    float colors[6][3] = {
        {0.5f, 0.0f, 0.7f},   // Purple
        {0.9f, 0.2f, 0.5f},   // Pink
        {1.0f, 0.5f, 0.1f},   // Orange
        {0.2f, 0.4f, 1.0f},   // Blue
        {0.6f, 0.2f, 0.8f},   // Magenta
        {0.3f, 0.7f, 0.9f}    // Cyan
    };

    for (int i = 0; i < NUM_NEBULA_PARTICLES; ++i) {
        // Random point in sphere (radius 2-5)
        float radius = 2.0f + (rand() % 300) / 100.0f;
        float theta = (rand() % 628) / 100.0f;      // 0 to 2π
        float phi = (rand() % 314) / 100.0f;        // 0 to π

        nebulaPart[i].originalX = radius * sinf(phi) * cosf(theta);
        nebulaPart[i].originalY = radius * sinf(phi) * sinf(theta);
        nebulaPart[i].originalZ = radius * cosf(phi);

        nebulaPart[i].x = nebulaPart[i].originalX;
        nebulaPart[i].y = nebulaPart[i].originalY;
        nebulaPart[i].z = nebulaPart[i].originalZ;

        // Random color from palette
        int colorIdx = rand() % 6;
        nebulaPart[i].r = colors[colorIdx][0];
        nebulaPart[i].g = colors[colorIdx][1];
        nebulaPart[i].b = colors[colorIdx][2];

        // Add slight color variation
        nebulaPart[i].r += (rand() % 100 - 50) / 500.0f;
        nebulaPart[i].g += (rand() % 100 - 50) / 500.0f;
        nebulaPart[i].b += (rand() % 100 - 50) / 500.0f;

        // Clamp colors to [0, 1]
        if (nebulaPart[i].r < 0) nebulaPart[i].r = 0;
        if (nebulaPart[i].r > 1) nebulaPart[i].r = 1;
        if (nebulaPart[i].g < 0) nebulaPart[i].g = 0;
        if (nebulaPart[i].g > 1) nebulaPart[i].g = 1;
        if (nebulaPart[i].b < 0) nebulaPart[i].b = 0;
        if (nebulaPart[i].b > 1) nebulaPart[i].b = 1;

        nebulaPart[i].alpha = 0.05f + (rand() % 150) / 1000.0f;
    }
}

// Update particles
void updateNebula() {
    for (int i = 0; i < NUM_NEBULA_PARTICLES; ++i) {
        // Swirling motion
        float swirl = sinf(globalTime * 0.3f + nebulaPart[i].y) * 0.01f;
        nebulaPart[i].x += swirl;

        swirl = cosf(globalTime * 0.3f + nebulaPart[i].z) * 0.01f;
        nebulaPart[i].y += swirl;

        swirl = sinf(globalTime * 0.3f + nebulaPart[i].x) * 0.01f;
        nebulaPart[i].z += swirl;

        // Expansion: slowly push particles outward from center
        float centerDist = sqrtf(nebulaPart[i].originalX * nebulaPart[i].originalX +
                                 nebulaPart[i].originalY * nebulaPart[i].originalY +
                                 nebulaPart[i].originalZ * nebulaPart[i].originalZ);

        if (centerDist > 0.01f) {
            float expandAmount = sinf(globalTime * 0.2f) * 0.001f + 0.0005f;
            nebulaPart[i].x += (nebulaPart[i].originalX / centerDist) * expandAmount;
            nebulaPart[i].y += (nebulaPart[i].originalY / centerDist) * expandAmount;
            nebulaPart[i].z += (nebulaPart[i].originalZ / centerDist) * expandAmount;
        }

        // Gentle oscillation in alpha for breathing effect
        nebulaPart[i].alpha = (0.05f + (rand() % 150) / 1000.0f) * 
                              (0.7f + 0.3f * sinf(globalTime * 0.5f));
    }
}

// Draw starfield
void drawStars() {
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_STARS; ++i) {
        float bright = stars[i].brightness + 0.2f * sinf(globalTime + i);
        if (bright > 1.0f) bright = 1.0f;
        glColor4f(bright, bright, bright, bright);
        glVertex3f(stars[i].x, stars[i].y, stars[i].z);
    }
    glEnd();
}

// Draw nebula cloud
void drawNebula() {
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_NEBULA_PARTICLES; ++i) {
        glColor4f(nebulaPart[i].r, nebulaPart[i].g, nebulaPart[i].b, nebulaPart[i].alpha);
        glVertex3f(nebulaPart[i].x, nebulaPart[i].y, nebulaPart[i].z);
    }
    glEnd();
}

// Draw proto-star core
void drawProtostar() {
    float pulseSize = 0.5f + 0.3f * sinf(globalTime * 0.7f);
    
    // Core glow
    int numSphereSegments = 16;
    glPointSize(8.0f + pulseSize * 4.0f);
    glBegin(GL_POINTS);
    glColor4f(1.0f, 0.9f, 0.6f, 0.8f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();

    // Bright sphere rings
    for (int ring = 0; ring < 3; ++ring) {
        float radius = 0.3f + ring * 0.3f;
        float alpha = 0.5f - ring * 0.15f;
        
        glBegin(GL_LINE_LOOP);
        glColor4f(1.0f, 0.8f, 0.4f, alpha);
        for (int i = 0; i < numSphereSegments; ++i) {
            float angle = (i / (float)numSphereSegments) * 6.28318f;
            float x = radius * cosf(angle);
            float y = radius * sinf(angle);
            glVertex3f(x, y, 0.0f);
        }
        glEnd();

        // Z-plane ring
        glBegin(GL_LINE_LOOP);
        glColor4f(1.0f, 0.7f, 0.3f, alpha * 0.7f);
        for (int i = 0; i < numSphereSegments; ++i) {
            float angle = (i / (float)numSphereSegments) * 6.28318f;
            float x = radius * cosf(angle);
            float z = radius * sinf(angle);
            glVertex3f(x, 0.0f, z);
        }
        glEnd();
    }

    // Pulsing corona particles
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 200; ++i) {
        float angle = (i / 200.0f) * 6.28318f;
        float dist = 0.8f + 0.4f * sinf(globalTime + i * 0.1f);
        float x = dist * cosf(angle);
        float y = dist * sinf(angle);
        float z = 0.5f * sinf(angle + globalTime);
        
        float alpha = 0.6f * (0.5f + 0.5f * sinf(globalTime + i * 0.05f));
        glColor4f(1.0f, 0.8f, 0.2f, alpha);
        glVertex3f(x, y, z);
    }
    glEnd();
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Rotate camera around nebula
    float camX = cameraDistance * cosf(cameraAngle);
    float camY = cameraDistance * 0.5f * sinf(globalTime * 0.15f);
    float camZ = cameraDistance * sinf(cameraAngle);

    gluLookAt(camX, camY, camZ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    cameraAngle += 0.003f;

    // Draw background
    glClearColor(0.0f, 0.0f, 0.01f, 1.0f);

    // Draw starfield
    drawStars();

    // Draw nebula
    drawNebula();

    // Draw proto-star
    drawProtostar();

    glFlush();
    glutSwapBuffers();
}

// Reshape callback
void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)w / (float)h, 0.1f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Idle callback - update animation
void idle() {
    globalTime += 0.016f;  // Assume 60 FPS
    updateNebula();
    glutPostRedisplay();
}

// Keyboard callback
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {  // ESC
        exit(0);
    }
    if (key == ' ') {  // SPACE - reset camera
        cameraAngle = 0.0f;
        globalTime = 0.0f;
    }
}

// Initialize OpenGL
void initGL() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // Additive blending for glow
    
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.01f, 1.0f);

    initStars();
    initNebula();
}

int main(int argc, char** argv) {
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Nebula Genesis - Cosmic Dance");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    // Print controls
    printf("=== Nebula Genesis ===\n");
    printf("Controls:\n");
    printf("  SPACE  - Reset camera and time\n");
    printf("  ESC    - Exit\n");
    printf("\nEnjoy the cosmic nebula formation!\n");

    glutMainLoop();

    return 0;
}
