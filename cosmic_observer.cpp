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
float sceneDuration = 20.0f;

// 3D scene rotation
float rotationX = 0.0f;
float rotationY = 0.0f;
float rotationZ = 0.0f;

// Star structure
struct Star {
    float x, y, z;
    float brightness;
    float size;
};

// Comet tail particle
struct TailParticle {
    float x, y, z;
    float r, g, b;
    float alpha;
    float age;
};

// Atmosphere particle (dust/fog)
struct AtmosphereParticle {
    float x, y, z;
    float r, g, b;
    float alpha;
};

// Meteor/comet streak
struct CometStreak {
    float startX, startY, startZ;
    float endX, endY, endZ;
    float r, g, b;
    float brightness;
    float age;
    float maxAge;
};

const int NUM_STARS = 8000;
const int NUM_TAIL_PARTICLES = 5000;
const int NUM_ATMOSPHERE = 6000;
const int NUM_STREAKS = 40;  // Many small comets

Star stars[NUM_STARS];
TailParticle tailParticles[NUM_TAIL_PARTICLES];
AtmosphereParticle atmosphere[NUM_ATMOSPHERE];
CometStreak streaks[NUM_STREAKS];
int nextTailIndex = 0;

// Initialize starfield with better 3D distribution
void initStars() {
    for (int i = 0; i < NUM_STARS; ++i) {
        // Much wider 3D space distribution
        stars[i].x = (rand() % 10000 - 5000) * 0.01f;
        stars[i].y = (rand() % 10000 - 5000) * 0.01f;
        stars[i].z = (rand() % 10000 - 5000) * 0.01f - 50.0f;
        
        // More varied brightness for depth perception
        stars[i].brightness = 0.05f + (rand() % 95) / 100.0f;
        stars[i].size = 0.3f + (rand() % 40) / 10.0f;
    }
}

// Initialize atmosphere with full 3D volumetric distribution
void initAtmosphere() {
    for (int i = 0; i < NUM_ATMOSPHERE; ++i) {
        // Full 3D sphere of atmosphere centered at origin
        float theta = (rand() % 36000) / 100.0f * 3.14159f / 180.0f;
        float phi = (rand() % 36000) / 100.0f * 3.14159f / 180.0f;
        float radius = 5.0f + (rand() % 50) * 0.1f;
        
        atmosphere[i].x = radius * sinf(phi) * cosf(theta);
        atmosphere[i].y = radius * sinf(phi) * sinf(theta);
        atmosphere[i].z = radius * cosf(phi);
        
        // Magenta/red/pink colors for nebula effect
        int colorType = rand() % 100;
        if (colorType < 35) {
            // Deep magenta
            atmosphere[i].r = 0.8f + (rand() % 20) / 100.0f;
            atmosphere[i].g = 0.1f + (rand() % 15) / 100.0f;
            atmosphere[i].b = 0.6f + (rand() % 30) / 100.0f;
        } else if (colorType < 65) {
            // Crimson/magenta
            atmosphere[i].r = 1.0f;
            atmosphere[i].g = 0.2f + (rand() % 20) / 100.0f;
            atmosphere[i].b = 0.4f + (rand() % 30) / 100.0f;
        } else {
            // Hot pink
            atmosphere[i].r = 1.0f;
            atmosphere[i].g = 0.3f + (rand() % 20) / 100.0f;
            atmosphere[i].b = 0.7f + (rand() % 20) / 100.0f;
        }
        
        atmosphere[i].alpha = 0.04f + (rand() % 80) / 1000.0f;
    }
}

// Initialize tail particles
void initTailParticles() {
    for (int i = 0; i < NUM_TAIL_PARTICLES; ++i) {
        tailParticles[i].x = 0;
        tailParticles[i].y = 0;
        tailParticles[i].z = 0;
        tailParticles[i].r = 1.0f;
        tailParticles[i].g = 0.5f;
        tailParticles[i].b = 0.8f;
        tailParticles[i].alpha = 0;
        tailParticles[i].age = 0;
    }
}

// Initialize comet streaks
void initStreaks() {
    for (int i = 0; i < NUM_STREAKS; ++i) {
        streaks[i].age = -1.0f;  // Inactive
        streaks[i].maxAge = 0;
    }
}

// Emit rotating nebula particles
void emitNebula() {
    // Rotating sphere of nebula particles
    for (int i = 0; i < NUM_ATMOSPHERE; ++i) {
        float rotX = rotationX * 3.14159f / 180.0f;
        float rotY = rotationY * 3.14159f / 180.0f;
        float rotZ = rotationZ * 3.14159f / 180.0f;
        
        // Apply rotation to particles
        float x = atmosphere[i].x;
        float y = atmosphere[i].y * cosf(rotX) - atmosphere[i].z * sinf(rotX);
        float z = atmosphere[i].y * sinf(rotX) + atmosphere[i].z * cosf(rotX);
        
        float x2 = x * cosf(rotY) + z * sinf(rotY);
        float z2 = -x * sinf(rotY) + z * cosf(rotY);
        
        atmosphere[i].x = x2 * cosf(rotZ) - y * sinf(rotZ);
        atmosphere[i].y = x2 * sinf(rotZ) + y * cosf(rotZ);
        atmosphere[i].z = z2;
    }
}

// Update 3D scene rotation
void updateScene() {
    // Smooth rotation around multiple axes
    rotationX += 0.02f;  // Rotate around X axis
    rotationY += 0.015f; // Rotate around Y axis
    rotationZ += 0.01f;  // Rotate around Z axis (slower)
    
    // Keep rotations in 0-360 range
    if (rotationX > 360.0f) rotationX -= 360.0f;
    if (rotationY > 360.0f) rotationY -= 360.0f;
    if (rotationZ > 360.0f) rotationZ -= 360.0f;
    
    // Emit rotating nebula effect
    emitNebula();
}

// Draw stellar environment - glowing 3D starfield with twinkling
void drawStarfield() {
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    
    for (int i = 0; i < NUM_STARS; ++i) {
        // Sophisticated twinkling based on distance and time
        float distanceDepth = 1.0f - (stars[i].z + 100.0f) / 200.0f;  // Closer = brighter twinkle
        float twinkle = 0.5f + 0.5f * sinf(globalTime * 1.5f + i * 0.01f);
        float brightness = stars[i].brightness * twinkle * (0.6f + distanceDepth * 0.4f);
        
        // Color variety - warmer distant stars, cooler near stars
        float warmth = (stars[i].z + 100.0f) / 200.0f;
        float r = brightness * (0.7f + warmth * 0.3f);
        float g = brightness * (0.6f + warmth * 0.1f);
        float b = brightness * (1.0f - warmth * 0.2f);
        
        glColor3f(r, g, b);
        glVertex3f(stars[i].x, stars[i].y, stars[i].z);
    }
    glEnd();
}

// Draw 3D rotating nebula/atmosphere with volumetric depth
void drawAtmosphere() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glPointSize(12.0f);
    glBegin(GL_POINTS);
    
    for (int i = 0; i < NUM_ATMOSPHERE; ++i) {
        // Apply rotation to get current particle position
        float rotX = rotationX * 3.14159f / 180.0f;
        float rotY = rotationY * 3.14159f / 180.0f;
        float rotZ = rotationZ * 3.14159f / 180.0f;
        
        float x = atmosphere[i].x;
        float y = atmosphere[i].y * cosf(rotX) - atmosphere[i].z * sinf(rotX);
        float z = atmosphere[i].y * sinf(rotX) + atmosphere[i].z * cosf(rotX);
        
        float x2 = x * cosf(rotY) + z * sinf(rotY);
        float z2 = -x * sinf(rotY) + z * cosf(rotY);
        
        float finalX = x2 * cosf(rotZ) - y * sinf(rotZ);
        float finalY = x2 * sinf(rotZ) + y * cosf(rotZ);
        float finalZ = z2;
        
        // Depth-based visibility and brightness
        float depthFade = 0.3f + 0.7f * (1.0f - (finalZ + 50.0f) / 100.0f);
        if (depthFade < 0.0f) depthFade = 0.0f;
        
        glColor4f(atmosphere[i].r, atmosphere[i].g, atmosphere[i].b, 
                 atmosphere[i].alpha * depthFade);
        glVertex3f(finalX, finalY, finalZ);
    }
    glEnd();
    
    glDisable(GL_BLEND);
}

// Draw central nebula core
void drawNebulaCor() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    // Outer corona
    glPointSize(35.0f);
    glBegin(GL_POINTS);
    glColor4f(1.0f, 0.4f, 0.8f, 0.2f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    
    // Middle glow
    glPointSize(25.0f);
    glBegin(GL_POINTS);
    glColor4f(1.0f, 0.6f, 0.9f, 0.4f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    
    // Inner core - bright center
    glPointSize(12.0f);
    glBegin(GL_POINTS);
    glColor4f(1.0f, 0.9f, 1.0f, 0.8f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    
    // Ultra-bright center
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    
    glDisable(GL_BLEND);
}

// Display callback

// Display callback
void display() {
    // Deep cosmic blue-black background
    float bgValue = 0.02f + 0.01f * sinf(globalTime * 0.2f);
    glClearColor(bgValue, bgValue * 0.5f, bgValue * 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Static camera in 3D space viewing rotating nebula
    gluLookAt(0.0f, 0.0f, 35.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    // Draw scene layers
    drawStarfield();
    drawAtmosphere();
    drawNebulaCor();

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
    gluPerspective(55.0f, (float)w / (float)h, 0.1f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Idle callback
void idle() {
    globalTime += 0.016f;
    
    if (globalTime > sceneDuration + 5.0f) {
        exit(0);
    }
    
    updateScene();
    glutPostRedisplay();
}

// Keyboard callback
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {  // ESC
        exit(0);
    }
    if (key == ' ') {  // SPACE - reset
        globalTime = 0.0f;
        rotationX = 0.0f;
        rotationY = 0.0f;
        rotationZ = 0.0f;
    }
}

// Initialize OpenGL
void initGL() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.08f, 0.0f, 0.01f, 1.0f);

    initStars();
    initAtmosphere();
}

int main(int argc, char** argv) {
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cosmic Observer - Dramatic Comet - Cosmic Dance");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    printf("=== Cosmic Observer ===\n");
    printf("A dramatic comet streaks across the cosmic stage...\n");
    printf("Scene duration: %.0f seconds\n", sceneDuration);
    printf("\nControls:\n");
    printf("  SPACE - Reset scene\n");
    printf("  ESC   - Exit\n");

    glutMainLoop();

    return 0;
}
