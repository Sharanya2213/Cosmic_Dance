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
float cameraDistance = 15.0f;
float cameraAngle = 0.0f;

// Star structures
struct BackgroundStar {
    float x, y, z;
    float brightness;
    float baseColor[3];
};

struct GalaxyStar {
    float x, y, z;
    float brightness;
    float baseColor[3];
    float orbitRadius;
    float orbitAngle;
    float orbitSpeed;
};

const int NUM_BACKGROUND_STARS = 5000;
const int NUM_GALAXY_STARS = 10000;

BackgroundStar backgroundStars[NUM_BACKGROUND_STARS];
GalaxyStar galaxyStars[NUM_GALAXY_STARS];

// Initialize background starfield
void initBackgroundStars() {
    for (int i = 0; i < NUM_BACKGROUND_STARS; ++i) {
        backgroundStars[i].x = (rand() % 4000 - 2000) * 0.1f;
        backgroundStars[i].y = (rand() % 4000 - 2000) * 0.1f;
        backgroundStars[i].z = (rand() % 4000 - 2000) * 0.1f;
        
        backgroundStars[i].brightness = 0.3f + (rand() % 70) / 100.0f;
        
        // Mostly white/blue
        int colorType = rand() % 100;
        if (colorType < 80) {
            // White/blue stars
            backgroundStars[i].baseColor[0] = 0.8f + (rand() % 20) / 100.0f;
            backgroundStars[i].baseColor[1] = 0.8f + (rand() % 20) / 100.0f;
            backgroundStars[i].baseColor[2] = 1.0f;
        } else if (colorType < 95) {
            // Yellow/orange
            backgroundStars[i].baseColor[0] = 1.0f;
            backgroundStars[i].baseColor[1] = 0.8f + (rand() % 20) / 100.0f;
            backgroundStars[i].baseColor[2] = 0.4f + (rand() % 20) / 100.0f;
        } else {
            // Red dwarf
            backgroundStars[i].baseColor[0] = 1.0f;
            backgroundStars[i].baseColor[1] = 0.4f + (rand() % 30) / 100.0f;
            backgroundStars[i].baseColor[2] = 0.3f + (rand() % 30) / 100.0f;
        }
    }
}

// Initialize spiral galaxy
void initGalaxyStars() {
    int starIdx = 0;
    
    // Create spiral arms
    int numArms = 3;
    
    for (int arm = 0; arm < numArms && starIdx < NUM_GALAXY_STARS; ++arm) {
        int starsPerArm = NUM_GALAXY_STARS / numArms;
        
        for (int i = 0; i < starsPerArm && starIdx < NUM_GALAXY_STARS; ++i) {
            // Random position in spiral
            float t = (i / (float)starsPerArm) * 10.0f;  // Parameter 0 to 10
            float radius = t * 0.5f;  // Gradually increase radius
            
            // Add randomness to radius
            float radiusVariation = (rand() % 30 - 15) / 100.0f;
            radius += radiusVariation;
            
            // Angle follows logarithmic spiral
            float angle = radius * 2.0f + (arm * 6.28318f / numArms);
            
            // Convert to Cartesian coordinates
            galaxyStars[starIdx].x = radius * cosf(angle);
            galaxyStars[starIdx].z = radius * sinf(angle);
            galaxyStars[starIdx].y = (rand() % 100 - 50) / 1000.0f;  // Thin disk
            
            // Store orbital parameters
            galaxyStars[starIdx].orbitRadius = radius;
            galaxyStars[starIdx].orbitAngle = angle;
            galaxyStars[starIdx].orbitSpeed = 2.0f / (1.0f + radius * 0.5f);  // Faster near center
            
            // Color: white/blue most common, yellow/orange near center
            if (radius < 1.5f) {
                // Core region: more yellow/orange
                int colorType = rand() % 100;
                if (colorType < 40) {
                    // Yellow
                    galaxyStars[starIdx].baseColor[0] = 1.0f;
                    galaxyStars[starIdx].baseColor[1] = 0.9f + (rand() % 10) / 100.0f;
                    galaxyStars[starIdx].baseColor[2] = 0.3f + (rand() % 20) / 100.0f;
                } else if (colorType < 70) {
                    // Orange
                    galaxyStars[starIdx].baseColor[0] = 1.0f;
                    galaxyStars[starIdx].baseColor[1] = 0.6f + (rand() % 20) / 100.0f;
                    galaxyStars[starIdx].baseColor[2] = 0.2f + (rand() % 20) / 100.0f;
                } else {
                    // White
                    galaxyStars[starIdx].baseColor[0] = 1.0f;
                    galaxyStars[starIdx].baseColor[1] = 0.95f + (rand() % 10) / 100.0f;
                    galaxyStars[starIdx].baseColor[2] = 0.9f + (rand() % 10) / 100.0f;
                }
            } else {
                // Outer regions: mostly white/blue
                int colorType = rand() % 100;
                if (colorType < 85) {
                    // Blue/white
                    galaxyStars[starIdx].baseColor[0] = 0.7f + (rand() % 30) / 100.0f;
                    galaxyStars[starIdx].baseColor[1] = 0.7f + (rand() % 30) / 100.0f;
                    galaxyStars[starIdx].baseColor[2] = 1.0f;
                } else {
                    // Yellow
                    galaxyStars[starIdx].baseColor[0] = 1.0f;
                    galaxyStars[starIdx].baseColor[1] = 0.8f + (rand() % 20) / 100.0f;
                    galaxyStars[starIdx].baseColor[2] = 0.4f + (rand() % 20) / 100.0f;
                }
            }
            
            // Brightness: brighter near center
            galaxyStars[starIdx].brightness = 0.6f + 0.4f / (1.0f + radius);
            
            starIdx++;
        }
    }
}

// Update galaxy rotation
void updateGalaxy() {
    float galaxyRotation = globalTime * 0.1f;  // Slow rotation
    
    for (int i = 0; i < NUM_GALAXY_STARS; ++i) {
        // Update orbital position
        float newAngle = galaxyStars[i].orbitAngle + galaxyStars[i].orbitSpeed * globalTime;
        
        galaxyStars[i].x = galaxyStars[i].orbitRadius * cosf(newAngle + galaxyRotation);
        galaxyStars[i].z = galaxyStars[i].orbitRadius * sinf(newAngle + galaxyRotation);
        
        // Twinkling: Add brightness variation for subset of stars
        if ((i % 5) == 0) {
            float twinkleFactor = 0.7f + 0.3f * sinf(globalTime * 2.0f + i * 0.1f);
            galaxyStars[i].brightness = (0.6f + 0.4f / (1.0f + galaxyStars[i].orbitRadius)) * twinkleFactor;
        }
    }
}

// Draw background starfield
void drawBackgroundStars() {
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    
    for (int i = 0; i < NUM_BACKGROUND_STARS; ++i) {
        // Twinkling effect
        float twinkle = 0.5f + 0.5f * sinf(globalTime * 1.5f + i * 0.05f);
        float brightness = backgroundStars[i].brightness * twinkle;
        
        glColor3f(
            backgroundStars[i].baseColor[0] * brightness,
            backgroundStars[i].baseColor[1] * brightness,
            backgroundStars[i].baseColor[2] * brightness
        );
        glVertex3f(backgroundStars[i].x, backgroundStars[i].y, backgroundStars[i].z);
    }
    glEnd();
}

// Draw spiral galaxy
void drawGalaxy() {
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    
    for (int i = 0; i < NUM_GALAXY_STARS; ++i) {
        glColor3f(
            galaxyStars[i].baseColor[0] * galaxyStars[i].brightness,
            galaxyStars[i].baseColor[1] * galaxyStars[i].brightness,
            galaxyStars[i].baseColor[2] * galaxyStars[i].brightness
        );
        glVertex3f(galaxyStars[i].x, galaxyStars[i].y, galaxyStars[i].z);
    }
    glEnd();
}

// Draw galactic core
void drawGalacticCore() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // Additive blending
    
    // Dense bright core
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    
    int coreParticles = 500;
    for (int i = 0; i < coreParticles; ++i) {
        float angle = (i / (float)coreParticles) * 6.28318f;
        float radius = (rand() % 100) / 200.0f;
        
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        float y = (rand() % 40 - 20) / 200.0f;
        
        // Bright yellow/white
        float intensity = 1.0f - (radius / 0.5f) * 0.5f;
        glColor4f(1.0f, 0.95f, 0.7f, 0.9f * intensity);
        glVertex3f(x, y, z);
    }
    glEnd();
    
    // Outer glow ring
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    
    for (int i = 0; i < 300; ++i) {
        float angle = globalTime * 0.5f + (i / 300.0f) * 6.28318f;
        float radius = 0.8f + 0.3f * sinf(globalTime + i * 0.02f);
        
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        float y = (rand() % 50 - 25) / 300.0f;
        
        float intensity = 0.5f + 0.5f * sinf(globalTime * 2.0f + i * 0.1f);
        glColor4f(1.0f, 0.8f, 0.3f, 0.6f * intensity);
        glVertex3f(x, y, z);
    }
    glEnd();
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Orbiting camera
    float camX = cameraDistance * cosf(cameraAngle);
    float camY = cameraDistance * 0.3f;
    float camZ = cameraDistance * sinf(cameraAngle);

    gluLookAt(camX, camY, camZ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    cameraAngle += 0.002f;

    // Draw scene
    drawBackgroundStars();
    drawGalaxy();
    drawGalacticCore();

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
        const char* titleText = "Chapter 5: The Milky Way Galaxy";
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
    updateGalaxy();
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
    if (key == '+' || key == '=') {  // Zoom in
        cameraDistance -= 1.0f;
        if (cameraDistance < 5.0f) cameraDistance = 5.0f;
    }
    if (key == '-' || key == '_') {  // Zoom out
        cameraDistance += 1.0f;
        if (cameraDistance > 50.0f) cameraDistance = 50.0f;
    }
}

// Initialize OpenGL
void initGL() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // Additive blending for glow
    
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Deep black

    initBackgroundStars();
    initGalaxyStars();
}

int main(int argc, char** argv) {
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Stars and the Milky Way - Cosmic Dance");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    // Print controls
    printf("=== Stars and the Milky Way ===\n");
    printf("Controls:\n");
    printf("  +/-   - Zoom in/out\n");
    printf("  SPACE - Reset camera and time\n");
    printf("  ESC   - Exit\n");
    printf("\nEnjoy the spiral galaxy!\n");

    glutMainLoop();

    return 0;
}
