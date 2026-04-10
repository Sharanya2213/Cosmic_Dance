#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

// ============================================================================
// REAL SOLAR SYSTEM DATA (Scaled for visualization)
// ============================================================================
struct PlanetData {
    const char* name;
    float orbitRadius;      // Distance from sun (AU * scale)
    float size;             // Planet radius
    float rotationPeriod;   // How fast it spins
    float orbitalPeriod;    // Time to orbit sun (in seconds at 1x speed)
    float tiltAngle;        // Axial tilt in degrees
    float r, g, b;          // Color
    bool hasRings;          // Saturn-like rings?
    float ringInnerRadius;  // Ring size
    float ringOuterRadius;
};

// Real solar system scaled down
PlanetData planets[] = {
    {"Mercury", 2.8f,   0.12f, 58.65f,  0.24f,   0.0f,   0.7f, 0.6f, 0.5f,  false, 0, 0},
    {"Venus",   4.2f,   0.18f, 243.0f,  0.62f,  177.0f,   1.0f, 0.9f, 0.7f,  false, 0, 0},
    {"Earth",   5.8f,   0.20f,  0.997f, 1.0f,   23.4f,    0.2f, 0.6f, 1.0f,  false, 0, 0},
    {"Mars",    7.5f,   0.15f,  1.03f,  1.88f,  25.2f,    1.0f, 0.3f, 0.1f,  false, 0, 0},
    {"Jupiter", 11.0f,  0.50f,  0.41f,  11.86f, 3.1f,     0.8f, 0.7f, 0.5f,  false, 0, 0},
    {"Saturn",  14.5f,  0.42f,  0.43f,  29.46f, 26.7f,    0.9f, 0.8f, 0.6f,  true,  0.65f, 0.95f},
    {"Uranus",  18.0f,  0.35f,  0.72f,  84.01f, 97.8f,    0.4f, 0.9f, 1.0f,  false, 0, 0},
    {"Neptune", 21.0f,  0.35f,  0.67f, 164.8f,  28.3f,    0.2f, 0.4f, 1.0f,  false, 0, 0},
};

// ============================================================================
// CUSTOM MATH
// ============================================================================
struct Vec3 {
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
};

// ============================================================================
// GLOBAL STATE
// ============================================================================
float globalTime = 0.0f;
float deltaTime = 0.016f;
float timeScale = 1.0f;  // Speed multiplier

// ============================================================================
// PROCEDURAL GEOMETRY
// ============================================================================
void drawSphere(float radius, int subdivisions) {
    for (int i = 0; i < subdivisions; ++i) {
        float lat1 = -1.5708f + (i / (float)subdivisions) * 3.14159f;
        float lat2 = -1.5708f + ((i + 1) / (float)subdivisions) * 3.14159f;
        
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= subdivisions; ++j) {
            float lng = -3.14159f + (j / (float)subdivisions) * 6.28318f;
            
            float x1 = radius * cosf(lat1) * cosf(lng);
            float y1 = radius * sinf(lat1);
            float z1 = radius * cosf(lat1) * sinf(lng);
            
            float x2 = radius * cosf(lat2) * cosf(lng);
            float y2 = radius * sinf(lat2);
            float z2 = radius * cosf(lat2) * sinf(lng);
            
            glNormal3f(x1 / radius, y1 / radius, z1 / radius);
            glVertex3f(x1, y1, z1);
            
            glNormal3f(x2 / radius, y2 / radius, z2 / radius);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }
}

void drawOrbit(float radius, int segments = 200) {
    glDisable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
    glLineWidth(1.0f);
    
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float angle = (i / (float)segments) * 6.28318f;
        glVertex3f(radius * cosf(angle), 0.0f, radius * sinf(angle));
    }
    glEnd();
    
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

void drawRings(float innerRadius, float outerRadius) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.9f, 0.8f, 0.6f, 0.6f);
    
    int segments = 100;
    float layers = 10;
    
    for (int layer = 0; layer < layers; ++layer) {
        float r_inner = innerRadius + (layer / layers) * (outerRadius - innerRadius);
        float r_outer = innerRadius + ((layer + 1) / layers) * (outerRadius - innerRadius);
        
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= segments; ++i) {
            float angle = (i / (float)segments) * 6.28318f;
            float a = 0.5f * (1.0f - layer / layers);  // Fade outer rings
            
            glColor4f(0.9f, 0.8f, 0.6f, a);
            glVertex3f(r_inner * cosf(angle), 0.02f, r_inner * sinf(angle));
            glVertex3f(r_outer * cosf(angle), 0.01f, r_outer * sinf(angle));
        }
        glEnd();
    }
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// ============================================================================
// STARFIELD
// ============================================================================
void drawStarfield() {
    glDisable(GL_LIGHTING);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    
    glColor3f(0.8f, 0.8f, 1.0f);
    for (int i = 0; i < 5000; ++i) {
        float x = -80.0f + (rand() % 16000) / 100.0f;
        float y = -50.0f + (rand() % 10000) / 100.0f;
        float z = -80.0f + (rand() % 16000) / 100.0f;
        
        float brightness = 0.5f + (rand() % 100) / 100.0f * 0.5f;
        glColor3f(brightness, brightness, brightness + 0.1f);
        glVertex3f(x, y, z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

// ============================================================================
// CENTRAL SUN - Accurate Like Reference Images
// ============================================================================
void drawSun() {
    glPushMatrix();
    
    // Main sun sphere
    glColor3f(1.0f, 0.95f, 0.7f);
    drawSphere(1.2f, 40);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDisable(GL_LIGHTING);
    
    // Corona layer 1: bright yellow
    glColor4f(1.0f, 0.9f, 0.2f, 0.7f);
    drawSphere(1.8f, 30);
    
    // Corona layer 2: orange glow
    glColor4f(1.0f, 0.6f, 0.1f, 0.5f);
    drawSphere(2.8f, 20);
    
    // Corona layer 3: dim outer glow
    glColor4f(1.0f, 0.3f, 0.0f, 0.2f);
    drawSphere(4.0f, 16);
    
    // Particle burst effect
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 800; ++i) {
        float angle = globalTime * timeScale * 2.0f + i * 0.008f;
        float elevation = (i / 800.0f) * 6.28318f;
        float radius = 1.8f + 1.5f * sinf(globalTime * timeScale + i * 0.05f);
        
        float x = radius * cosf(elevation) * cosf(angle);
        float y = radius * sinf(elevation);
        float z = radius * cosf(elevation) * sinf(angle);
        
        float intensity = 1.0f - (radius / 3.5f);
        glColor4f(1.0f, 0.8f, 0.3f * intensity, 0.8f * intensity);
        glVertex3f(x, y, z);
    }
    glEnd();
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    
    glPopMatrix();
}

// ============================================================================
// DUST/NEBULA EFFECT AROUND ORBITS
// ============================================================================
void drawDustClouds() {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glPointSize(0.8f);
    
    glBegin(GL_POINTS);
    for (int i = 0; i < 20000; ++i) {
        float angle = globalTime * timeScale * 0.1f + (rand() % 360) * 0.017f;
        float radius = 3.0f + (rand() % 400) / 100.0f;
        float height = (rand() % 100) / 100.0f - 0.5f;
        
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        float y = height * 0.3f;
        
        float dist_from_orbit = fabs(sqrt(x*x + z*z) - radius);
        float alpha = 0.1f * expf(-dist_from_orbit * dist_from_orbit / 0.5f);
        
        // Dust color: brownish
        glColor4f(0.7f, 0.6f, 0.4f, alpha);
        glVertex3f(x, y, z);
    }
    glEnd();
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// ============================================================================
// PLANET RENDERING
// ============================================================================
void drawPlanets() {
    int numPlanets = sizeof(planets) / sizeof(PlanetData);
    
    for (int i = 0; i < numPlanets; ++i) {
        PlanetData& p = planets[i];
        
        // Draw orbital path
        drawOrbit(p.orbitRadius);
        
        // Calculate position in orbit
        float orbitalPosition = fmodf(globalTime * timeScale / p.orbitalPeriod, 1.0f) * 6.28318f;
        float x = p.orbitRadius * cosf(orbitalPosition);
        float z = p.orbitRadius * sinf(orbitalPosition);
        
        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        
        // Apply axial tilt
        glRotatef(p.tiltAngle, cosf(orbitalPosition), 0.0f, sinf(orbitalPosition));
        
        // Rotation of planet
        glRotatef(globalTime * timeScale * 360.0f / p.rotationPeriod, 0.0f, 1.0f, 0.0f);
        
        // Draw planet
        glColor3f(p.r, p.g, p.b);
        drawSphere(p.size, 32);
        
        // Draw rings if applicable
        if (p.hasRings) {
            glPushMatrix();
            glRotatef(p.tiltAngle * 0.5f, 1.0f, 0.0f, 0.0f);
            drawRings(p.ringInnerRadius, p.ringOuterRadius);
            glPopMatrix();
        }
        
        glPopMatrix();
    }
}

// ============================================================================
// DISPLAY & UPDATE
// ============================================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Dynamic cinematic camera
    float camDistance = 35.0f;
    float camAngle = globalTime * timeScale * 0.15f;
    float camHeight = 8.0f + 3.0f * sinf(globalTime * timeScale * 0.3f);
    
    gluLookAt(
        camDistance * cosf(camAngle), 
        camHeight, 
        camDistance * sinf(camAngle),
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );
    
    // Render scene
    drawStarfield();
    drawDustClouds();
    drawSun();
    drawPlanets();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0f, (float)w / (float)h, 0.1f, 500.0f);
}

void idle() {
    globalTime += deltaTime;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
            timeScale = 0.5f;
            std::cout << "Speed: 0.5x (Slow, Realistic)" << std::endl;
            break;
        case '2':
            timeScale = 1.0f;
            std::cout << "Speed: 1.0x (Normal)" << std::endl;
            break;
        case '3':
            timeScale = 2.0f;
            std::cout << "Speed: 2.0x (Fast)" << std::endl;
            break;
        case '4':
            timeScale = 5.0f;
            std::cout << "Speed: 5.0x (Very Fast)" << std::endl;
            break;
        case ' ':
            globalTime = 0.0f;
            std::cout << "Reset to start" << std::endl;
            break;
        case 27:  // ESC
            exit(0);
            break;
    }
}

// ============================================================================
// MAIN
// ============================================================================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1600, 900);
    glutCreateWindow("ACCURATE SOLAR SYSTEM - 8 Planets with Real Orbital Mechanics");
    
    glClearColor(0.0f, 0.0f, 0.01f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_BLEND);
    
    // Setup lighting
    GLfloat light_pos[] = {0.0f, 2.0f, 1.0f, 0.0f};
    GLfloat light_ambient[] = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    
    std::cout << "====== ACCURATE SOLAR SYSTEM ======" << std::endl;
    std::cout << "Press 1 - Speed: 0.5x (Realistic)" << std::endl;
    std::cout << "Press 2 - Speed: 1.0x (Normal)" << std::endl;
    std::cout << "Press 3 - Speed: 2.0x (Fast)" << std::endl;
    std::cout << "Press 4 - Speed: 5.0x (Very Fast)" << std::endl;
    std::cout << "Press SPACE - Reset animation" << std::endl;
    std::cout << "Press ESC - Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "All planets have REAL orbital periods:" << std::endl;
    std::cout << "  Mercury: 0.24 seconds (fastest)" << std::endl;
    std::cout << "  Earth:   1.0 second (baseline)" << std::endl;
    std::cout << "  Saturn:  29.46 seconds (with RINGS)" << std::endl;
    std::cout << "  Neptune: 164.8 seconds (slowest)" << std::endl;
    
    glutMainLoop();
    return 0;
}

