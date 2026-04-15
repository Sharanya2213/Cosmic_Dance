#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

// ============================================================================
// GLOBAL CONSTANTS & VARIABLES
// ============================================================================

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const int NUM_PARTICLES = 2000;

float globalTime = 0.0f;
float deltaTime = 0.016f; // ~60 FPS

// Camera
float cameraX = 0.0f, cameraY = 0.5f, cameraZ = 3.0f;

// ============================================================================
// PARTICLE STRUCTURE & SYSTEM
// ============================================================================

struct Particle {
    float x, y, z;        // Position
    float vx, vy, vz;     // Velocity
    float r, g, b;        // Color (RGB)
    float alpha;          // Alpha/opacity
    float angle1, angle2; // For swirling motion
    float speed;          // Movement speed multiplier
    float lifetime;       // Particle age
    int type;             // 0=chaos, 1=nebula, 2=wave
};

std::vector<Particle> particles;
std::vector<Particle> nebulaClouds;

void initializeParticles() {
    particles.clear();
    nebulaClouds.clear();
    srand(static_cast<unsigned>(time(0)));
    
    // Create chaos particles (main swirling system)
    for (int i = 0; i < NUM_PARTICLES * 0.7f; ++i) {
        Particle p;
        p.x = -1.0f + (rand() % 1000) / 500.0f;
        p.y = -1.0f + (rand() % 1000) / 500.0f;
        p.z = -1.0f + (rand() % 1000) / 500.0f;
        
        p.vx = -0.3f + (rand() % 1000) / 1666.67f;
        p.vy = -0.3f + (rand() % 1000) / 1666.67f;
        p.vz = -0.3f + (rand() % 1000) / 1666.67f;
        
        float colorChoice = rand() % 100 / 100.0f;
        p.r = 0.3f + colorChoice * 0.4f;
        p.g = 0.0f + colorChoice * 0.2f;
        p.b = 0.4f - colorChoice * 0.3f;
        
        p.alpha = 0.1f + (rand() % 200) / 1000.0f;
        p.angle1 = rand() % 360;
        p.angle2 = rand() % 360;
        p.speed = 0.5f + (rand() % 100) / 100.0f;
        p.lifetime = 0.0f;
        p.type = 0; // Chaos
        
        particles.push_back(p);
    }
    
    // Create wave particles (orbital patterns)
    for (int i = 0; i < NUM_PARTICLES * 0.3f; ++i) {
        Particle p;
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float radius = 0.5f + (rand() % 100) / 200.0f;
        
        p.x = cos(angle) * radius;
        p.y = sin(angle) * radius;
        p.z = -0.5f + (rand() % 1000) / 1000.0f;
        
        p.vx = 0.0f;
        p.vy = 0.0f;
        p.vz = 0.0f;
        
        p.r = 0.5f + (rand() % 100) / 200.0f;
        p.g = 0.2f + (rand() % 100) / 500.0f;
        p.b = 0.8f - (rand() % 100) / 500.0f;
        
        p.alpha = 0.15f + (rand() % 150) / 1000.0f;
        p.angle1 = angle * 180.0f / 3.14159f;
        p.angle2 = radius;
        p.speed = 1.0f + (rand() % 100) / 100.0f;
        p.lifetime = 0.0f;
        p.type = 2; // Wave
        
        particles.push_back(p);
    }
    
    // Create nebula clouds (background wisps)
    for (int i = 0; i < 500; ++i) {
        Particle cloud;
        cloud.x = -2.5f + (rand() % 5000) / 1000.0f;
        cloud.y = -2.5f + (rand() % 5000) / 1000.0f;
        cloud.z = -3.0f - (rand() % 2000) / 1000.0f;
        
        cloud.vx = (rand() % 100 - 50) / 500.0f;
        cloud.vy = (rand() % 100 - 50) / 500.0f;
        cloud.vz = (rand() % 100) / 1000.0f;
        
        // Nebula colors: deep blues, purples, dark reds
        int colorType = rand() % 3;
        if (colorType == 0) {
            cloud.r = 0.2f + (rand() % 100) / 500.0f;
            cloud.g = 0.1f;
            cloud.b = 0.4f + (rand() % 100) / 500.0f;
        } else if (colorType == 1) {
            cloud.r = 0.3f + (rand() % 100) / 500.0f;
            cloud.g = 0.0f;
            cloud.b = 0.3f + (rand() % 100) / 500.0f;
        } else {
            cloud.r = 0.4f + (rand() % 100) / 500.0f;
            cloud.g = 0.05f;
            cloud.b = 0.2f + (rand() % 100) / 500.0f;
        }
        
        cloud.alpha = 0.05f + (rand() % 100) / 1000.0f;
        cloud.angle1 = rand() % 360;
        cloud.angle2 = rand() % 360;
        cloud.speed = 0.1f + (rand() % 50) / 200.0f;
        cloud.lifetime = 0.0f;
        cloud.type = 1; // Nebula
        
        nebulaClouds.push_back(cloud);
    }
}

void updateParticles() {
    // Update chaos and wave particles
    for (auto& p : particles) {
        if (p.type == 0) { // Chaos particles
            float angle1Rad = (p.angle1 + globalTime * 30.0f) * 3.14159f / 180.0f;
            float angle2Rad = (p.angle2 + globalTime * 20.0f) * 3.14159f / 180.0f;
            
            float curveX = sin(angle1Rad) * 0.02f;
            float curveY = cos(angle2Rad) * 0.02f;
            float curveZ = sin(angle1Rad + angle2Rad) * 0.02f;
            
            p.vx += curveX;
            p.vy += curveY;
            p.vz += curveZ;
            
            float speedMultiplier = 1.0f + globalTime * 0.05f;
            p.x += p.vx * p.speed * speedMultiplier * deltaTime;
            p.y += p.vy * p.speed * speedMultiplier * deltaTime;
            p.z += p.vz * p.speed * speedMultiplier * deltaTime;
            
            if (p.x > 2.0f) p.x = -2.0f;
            if (p.x < -2.0f) p.x = 2.0f;
            if (p.y > 2.0f) p.y = -2.0f;
            if (p.y < -2.0f) p.y = 2.0f;
            if (p.z > 2.0f) p.z = -2.0f;
            if (p.z < -2.0f) p.z = 2.0f;
            
            p.angle1 += 1.0f;
            p.angle2 += 0.5f;
        } 
        else if (p.type == 2) { // Wave particles - orbital motion
            float orbitAngle = (p.angle1 + globalTime * 45.0f * p.speed) * 3.14159f / 180.0f;
            float radius = p.angle2;
            
            // Orbital motion with depth variation
            p.x = cos(orbitAngle) * radius;
            p.y = sin(orbitAngle) * radius;
            p.z = sin(orbitAngle * 0.5f) * 1.5f - 0.5f;
            
            // Pulsing alpha based on orbital position
            p.alpha = 0.15f + sin(orbitAngle) * 0.1f;
            if (p.alpha < 0.05f) p.alpha = 0.05f;
            
            // Color shift based on position
            p.r = 0.5f + sin(orbitAngle) * 0.3f;
            p.g = 0.1f + cos(orbitAngle) * 0.1f;
        }
        
        p.lifetime += deltaTime;
    }
    
    // Update nebula clouds
    for (auto& cloud : nebulaClouds) {
        // Slow drifting motion
        float driftX = sin(globalTime * 0.3f + cloud.angle1) * 0.005f;
        float driftY = cos(globalTime * 0.25f + cloud.angle2) * 0.005f;
        
        cloud.x += driftX + cloud.vx * deltaTime;
        cloud.y += driftY + cloud.vy * deltaTime;
        cloud.z += cloud.vz * deltaTime;
        
        // Wrap around
        if (cloud.x > 3.0f) cloud.x = -3.0f;
        if (cloud.x < -3.0f) cloud.x = 3.0f;
        if (cloud.y > 3.0f) cloud.y = -3.0f;
        if (cloud.y < -3.0f) cloud.y = 3.0f;
        
        // Pulsing alpha
        cloud.alpha = (0.05f + (rand() % 50) / 1000.0f) * (1.0f + sin(globalTime + cloud.angle1) * 0.5f);
    }
}

void drawParticles() {
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        glColor4f(p.r, p.g, p.b, p.alpha);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    
    // Draw nebula clouds with larger points
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    for (const auto& cloud : nebulaClouds) {
        glColor4f(cloud.r, cloud.g, cloud.b, cloud.alpha);
        glVertex3f(cloud.x, cloud.y, cloud.z);
    }
    glEnd();
}

// ============================================================================
// GEOMETRIC SHAPES
// ============================================================================

void drawRotatingCube(float rotation) {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -1.2f);
    glRotatef(rotation, 1.0f, 1.0f, 1.0f);
    
    glColor4f(0.3f, 0.1f, 0.5f, 0.15f);
    
    float size = 0.4f;
    glBegin(GL_LINE_LOOP);
    glVertex3f(-size, -size, -size);
    glVertex3f(size, -size, -size);
    glVertex3f(size, size, -size);
    glVertex3f(-size, size, -size);
    glEnd();
    
    glBegin(GL_LINE_LOOP);
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);
    glVertex3f(size, size, size);
    glVertex3f(-size, size, size);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, -size);
    glVertex3f(size, -size, size);
    glVertex3f(size, size, -size);
    glVertex3f(size, size, size);
    glVertex3f(-size, size, -size);
    glVertex3f(-size, size, size);
    glEnd();
    
    glPopMatrix();
}

void drawOrbitalRings() {
    glColor4f(0.4f, 0.2f, 0.6f, 0.2f);
    
    // Draw 3 rotating rings at different angles
    for (int ring = 0; ring < 3; ++ring) {
        glPushMatrix();
        
        if (ring == 0) {
            glRotatef(globalTime * 20.0f, 1.0f, 0.0f, 0.0f);
        } else if (ring == 1) {
            glRotatef(globalTime * 15.0f, 0.0f, 1.0f, 0.0f);
        } else {
            glRotatef(globalTime * 25.0f, 0.0f, 0.0f, 1.0f);
        }
        
        float radius = 1.0f + ring * 0.5f;
        int segments = 64;
        
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; ++i) {
            float angle = i * 2.0f * 3.14159f / segments;
            glVertex3f(cos(angle) * radius, sin(angle) * radius, 0.0f);
        }
        glEnd();
        
        glPopMatrix();
    }
}

void drawCenterSphere() {
    if (globalTime < 35.0f) return;
    
    float age = globalTime - 35.0f;
    float scale = 0.1f + age * 0.02f;
    if (scale > 0.5f) scale = 0.5f;
    
    float glow = 0.5f + sin(globalTime * 8.0f) * 0.3f;
    
    glPushMatrix();
    glColor4f(0.6f, 0.3f, 0.2f, 0.4f * glow);
    
    int slices = 16;
    int stacks = 16;
    
    // Draw wireframe sphere
    for (int i = 0; i < stacks; ++i) {
        float lat0 = 3.14159f * (-0.5f + (float)i / stacks);
        
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j <= slices; ++j) {
            float lon = 2.0f * 3.14159f * j / slices;
            float x = cos(lat0) * cos(lon) * scale;
            float y = sin(lat0) * scale;
            float z = cos(lat0) * sin(lon) * scale;
            glVertex3f(x, y, z);
        }
        glEnd();
    }
    
    glPopMatrix();
}

// ============================================================================
// LIGHTNING EFFECT
// ============================================================================

struct Lightning {
    float startX, startY, startZ;
    float endX, endY, endZ;
    float birthTime;
    float duration; // ~0.8 seconds
    bool active;
};

std::vector<Lightning> lightnings;
float lastLightningTime = -15.0f;

void generateLightning() {
    Lightning l;
    l.startX = -2.0f + (rand() % 4000) / 1000.0f;
    l.startY = -1.5f + (rand() % 3000) / 1000.0f;
    l.startZ = -2.0f + (rand() % 4000) / 1000.0f;
    
    l.endX = -2.0f + (rand() % 4000) / 1000.0f;
    l.endY = -1.5f + (rand() % 3000) / 1000.0f;
    l.endZ = -2.0f + (rand() % 4000) / 1000.0f;
    
    l.birthTime = globalTime;
    l.duration = 0.8f;
    l.active = true;
    
    lightnings.push_back(l);
}

void updateLightning() {
    // Generate new lightning every 6-10 seconds
    if (globalTime - lastLightningTime > (6.0f + (rand() % 4000) / 1000.0f)) {
        generateLightning();
        lastLightningTime = globalTime;
    }
    
    // Update existing lightnings
    std::vector<Lightning> activeLightnings;
    for (auto& l : lightnings) {
        if (globalTime - l.birthTime < l.duration) {
            activeLightnings.push_back(l);
        }
    }
    lightnings = activeLightnings;
}

void drawLightning() {
    for (const auto& l : lightnings) {
        float elapsed = globalTime - l.birthTime;
        float progress = elapsed / l.duration;
        
        // Color transition: white -> purple -> fade out
        float r, g, b, a;
        if (progress < 0.5f) {
            // White to purple
            float t = progress * 2.0f;
            r = 1.0f - t * 0.3f;
            g = 1.0f - t * 1.0f;
            b = 1.0f;
            a = 0.8f;
        } else {
            // Purple to fade out
            float t = (progress - 0.5f) * 2.0f;
            r = 0.7f * (1.0f - t);
            g = 0.0f;
            b = 1.0f * (1.0f - t);
            a = 0.8f * (1.0f - t);
        }
        
        // Flicker effect
        if (fmod(globalTime * 10.0f, 1.0f) < 0.3f) {
            a *= 0.5f;
        }
        
        glColor4f(r, g, b, a);
        glLineWidth(2.5f);
        glBegin(GL_LINE_STRIP);
        
        // Draw jagged lightning line
        int segments = 8;
        for (int i = 0; i <= segments; ++i) {
            float t = i / (float)segments;
            float x = l.startX + (l.endX - l.startX) * t;
            float y = l.startY + (l.endY - l.startY) * t;
            float z = l.startZ + (l.endZ - l.startZ) * t;
            
            // Add jitter
            if (i > 0 && i < segments) {
                x += (sin(globalTime * 20.0f + i) * 0.15f);
                y += (cos(globalTime * 20.0f + i + 1.0f) * 0.15f);
                z += (sin(globalTime * 20.0f + i + 2.0f) * 0.15f);
            }
            
            glVertex3f(x, y, z);
        }
        glEnd();
        glLineWidth(1.0f);
    }
}

// ============================================================================
// CENTER LIGHT (GLOWING POINT)
// ============================================================================

void drawCenterLight() {
    if (globalTime < 40.0f) return; // Appears after 40 seconds
    
    float age = globalTime - 40.0f;
    float maxSize = 0.8f;
    float size = (age / 15.0f) * maxSize; // Grow over 15 seconds
    if (size > maxSize) size = maxSize;
    
    // Flickering using high frequency sine
    float flicker = 1.0f + sin(globalTime * 15.0f) * 0.3f;
    float scale = size * flicker;
    
    // Draw main glowing point
    glPointSize(scale * 20.0f);
    glColor4f(1.0f, 0.5f, 0.0f, 0.8f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    
    // Draw expanding halos
    for (int i = 0; i < 3; ++i) {
        float haloSize = scale * (30.0f + i * 20.0f);
        float haloAlpha = 0.3f / (i + 1);
        glPointSize(haloSize);
        glColor4f(1.0f, 0.3f, 0.0f, haloAlpha);
        glBegin(GL_POINTS);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
    }
    
    // Draw pulsing rings around center
    glColor4f(0.8f, 0.4f, 0.1f, 0.4f);
    for (int ring = 0; ring < 3; ++ring) {
        float ringRadius = 0.3f + ring * 0.2f + sin(globalTime * 3.0f + ring) * 0.1f;
        int segments = 32;
        
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; ++i) {
            float angle = i * 2.0f * 3.14159f / segments;
            glVertex3f(cos(angle) * ringRadius, sin(angle) * ringRadius, sin(angle * 2.0f) * ringRadius * 0.3f);
        }
        glEnd();
    }
}

// ============================================================================
// TEXT RENDERING
// ============================================================================

void drawText(float x, float y, float z, const char* text, void* font) {
    glRasterPos3f(x, y, z);
    for (int i = 0; text[i] != '\0'; ++i) {
        glutBitmapCharacter(font, text[i]);
    }
}

void drawTitleText() {
    if (globalTime > 4.5f) return; // Show for ~4 seconds
    
    // Fade in/out effect
    float alpha = 1.0f;
    if (globalTime < 0.5f) {
        alpha = globalTime / 0.5f; // Fade in
    } else if (globalTime > 4.0f) {
        alpha = (4.5f - globalTime) / 0.5f; // Fade out
    }
    
    // Pulsing glow effect - yellow/white for Big Bang
    float glow = 1.0f + sin(globalTime * 4.0f) * 0.3f;
    glColor4f(1.0f, 0.9f, 0.3f, alpha * glow);
    
    // Disable depth test for text rendering
    glDisable(GL_DEPTH_TEST);
    
    // Set up orthographic projection for text
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    drawText(-0.25f, 0.3f, -0.9f, "THE BIG BANG", GLUT_BITMAP_TIMES_ROMAN_24);
    
    // Restore matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

// ============================================================================
// FINALE EFFECT (19-25 seconds)
// ============================================================================

void drawFinaleEffect() {
    if (globalTime < 19.0f) return;  // Finale starts at 19 seconds
    
    float finaleTime = globalTime - 19.0f;
    if (finaleTime > 6.0f) finaleTime = 6.0f;
    
    float crescendo = finaleTime / 6.0f;
    
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Brilliant flash center
    glPointSize(crescendo * 100.0f);
    glColor4f(1.0f, 0.8f, 0.6f, (1.0f - crescendo) * 0.7f);
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    
    // Expanding rings of light
    int rings = 20;
    for (int i = 0; i < rings; ++i) {
        float ringAlpha = (1.0f - (float)i / rings) * (1.0f - crescendo) * 0.6f;
        float ringRadius = crescendo * 1.2f * (i / (float)rings);
        
        glColor4f(1.0f, 0.6f, 0.2f, ringAlpha);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < 64; ++j) {
            float angle = j * 2.0f * 3.14159f / 64;
            glVertex2f(cos(angle) * ringRadius, sin(angle) * ringRadius);
        }
        glEnd();
    }
    glLineWidth(1.0f);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

// ============================================================================
// BREATHING BACKGROUND
// ============================================================================

void updateBreathingBackground() {
    // Period of ~8 seconds
    float breath = sin((globalTime / 8.0f) * 2.0f * 3.14159f) * 0.5f + 0.5f;
    
    // Colors: black to very dark indigo
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.05f + breath * 0.08f; // 0.05 to 0.13
    
    glClearColor(r, g, b, 1.0f);
}

// ============================================================================
// OPENGL SETUP & CALLBACKS
// ============================================================================

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Glow-like effect
    
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    
    initializeParticles();
}

void display() {
    updateBreathingBackground();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ,  // Eye position
              0.0f, 0.0f, 0.0f,            // Look at
              0.0f, 1.0f, 0.0f);            // Up vector
    
    // Draw background geometric shapes
    drawRotatingCube(globalTime * 25.0f);
    drawOrbitalRings();
    
    // Draw all particle effects
    drawParticles();
    drawLightning();
    
    // Draw center light and sphere
    drawCenterSphere();
    drawCenterLight();
    
    drawTitleText();
    drawFinaleEffect();
    
    glutSwapBuffers();
}

void updateFrame() {
    globalTime += deltaTime;
    
    // Auto-exit Chapter 1 after 25 seconds
    if (globalTime > 25.0f) {
        globalTime = 25.0f;  // Hold at 25 seconds
    }
    
    // Update systems
    updateParticles();
    updateLightning();
    
    glutPostRedisplay();
}

void timer(int value) {
    updateFrame();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC
        exit(0);
    }
}

// ============================================================================
// MAIN
// ============================================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Chaos and Darkness - 3D Animation");
    
    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    // Initialize OpenGL
    init();
    
    // Start timer
    glutTimerFunc(16, timer, 0);
    
    std::cout << "Controls: Press ESC to exit" << std::endl;
    std::cout << "\nChaos and Darkness 3D Animation - Enhanced Version" << std::endl;
    std::cout << "=================================================" << std::endl;
    std::cout << "\nVisual Effects:" << std::endl;
    std::cout << "- Breathing dark background (8-second cycle)" << std::endl;
    std::cout << "- 2000+ particles: chaos swirls + orbital waves" << std::endl;
    std::cout << "- Nebula clouds drifting in background" << std::endl;
    std::cout << "- Rotating wireframe cube and rings" << std::endl;
    std::cout << "- Random lightning strikes every 6-10 seconds" << std::endl;
    std::cout << "- Glowing center sphere (appears after 35 seconds)" << std::endl;
    std::cout << "- Expanding center light with pulsing rings (after 40 seconds)" << std::endl;
    std::cout << "- Title fade-in/out (first 4 seconds)" << std::endl;
    
    glutMainLoop();
    return 0;
}
