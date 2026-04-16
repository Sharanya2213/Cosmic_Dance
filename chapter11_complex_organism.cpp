#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Global variables
float globalTime = 0.0f;

// Particle system for underwater effect
struct Particle {
    float x, y, z;
    float vx, vy, vz;
};

struct Fish {
    float x, y, z;
    float speed;
    float direction;
    float waveOffset;
};

std::vector<Particle> particles;
std::vector<Fish> fishes;

// Initialize particles - bubbles from ocean bottom
void initializeParticles() {
    particles.clear();
    for (int i = 0; i < 500; i++) {
        Particle p;
        // Start bubbles at bottom, spread across ocean floor
        p.x = (rand() % 400 - 200) / 10.0f;
        p.y = (rand() % 50 - 40) / 10.0f;  // Start near bottom
        p.z = (rand() % 150 - 75) / 10.0f;
        p.vx = (rand() % 30 - 15) / 2000.0f;
        p.vy = 0.01f;  // Will be set in draw
        p.vz = (rand() % 30 - 15) / 2000.0f;
        particles.push_back(p);
    }
}

// Initialize multiple fish
void initializeFishes() {
    fishes.clear();
    
    // Fish 1 - large, swimming right
    Fish f1 = {-8.0f, 1.0f, 0.0f, 0.8f, 0.0f, 0.0f};
    fishes.push_back(f1);
    
    // Fish 2 - medium, swimming left
    Fish f2 = {6.0f, -1.5f, 3.0f, -0.6f, 180.0f, 1.5f};
    fishes.push_back(f2);
    
    // Fish 3 - small, swimming right
    Fish f3 = {-5.0f, -3.0f, -2.5f, 1.0f, 0.0f, 2.5f};
    fishes.push_back(f3);
    
    // Fish 4 - medium, swimming up-right
    Fish f4 = {8.0f, 2.5f, -3.0f, 0.7f, 0.0f, 3.5f};
    fishes.push_back(f4);
}

// Draw particles - water current effects and light rays
void drawParticles() {
    glDisable(GL_LIGHTING);
    
    // Draw underwater light rays (caustics simulation)
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    
    for (auto& p : particles) {
        // Continuous flowing water current
        p.x += sinf(globalTime * 0.5f) * 0.002f;
        p.y += 0.005f;  // Gentle upward current
        p.z += cosf(globalTime * 0.4f) * 0.002f;
        
        // Seamless wrap-around for infinite ocean effect
        if (p.x > 40.0f) p.x = -40.0f;
        if (p.x < -40.0f) p.x = 40.0f;
        if (p.y > 30.0f) p.y = -30.0f;
        if (p.z > 40.0f) p.z = -40.0f;
        if (p.z < -40.0f) p.z = 40.0f;
        
        // Light rays - brighter near surface
        float distToSurface = 30.0f - p.y;
        float brightness = 0.15f + (distToSurface / 60.0f) * 0.35f;
        brightness *= 0.5f + 0.3f * sinf(globalTime * 1.5f + p.x * 0.1f);
        
        glColor4f(0.3f, 0.6f, 0.85f, brightness * 0.3f);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    
    // Draw caustic patterns (flickering light spots)
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    for (size_t i = 0; i < particles.size(); i += 5) {
        auto& p = particles[i];
        float causticBrightness = 0.2f + 0.3f * sinf(globalTime * 2.0f + i);
        glColor4f(0.4f, 0.7f, 0.95f, causticBrightness * 0.2f);
        glVertex3f(p.x, p.y + 2.0f, p.z);
    }
    glEnd();
    
    glPointSize(1.0f);
    glEnable(GL_LIGHTING);
}

// Draw underwater background and environment - seamless infinite ocean
void drawUnderwater() {
    glDisable(GL_LIGHTING);
    
    // Create seamless ocean environment with no visible edges
    // Large sphere that contains everything - deep ocean blue
    glPushMatrix();
    glColor4f(0.0f, 0.08f, 0.18f, 1.0f);
    glutSolidSphere(200.0f, 32, 32);
    glPopMatrix();
    
    // Animated water surface - waves rippling from above
    glPushMatrix();
    glTranslatef(0.0f, 35.0f, 0.0f);
    
    glBegin(GL_QUADS);
    float waveHeight = 2.0f;
    for (int i = -100; i < 100; i += 5) {
        for (int j = -100; j < 100; j += 5) {
            float wave1 = sinf(globalTime * 1.2f + i * 0.1f) * waveHeight;
            float wave2 = cosf(globalTime * 0.8f + j * 0.08f) * waveHeight * 0.7f;
            float waveTotal = wave1 + wave2;
            
            glColor4f(0.15f, 0.35f, 0.55f, 0.4f);
            glVertex3f(i, waveTotal, j);
            glVertex3f(i + 5, waveTotal, j);
            
            glColor4f(0.12f, 0.32f, 0.52f, 0.35f);
            glVertex3f(i + 5, waveTotal, j + 5);
            glVertex3f(i, waveTotal, j + 5);
        }
    }
    glEnd();
    glPopMatrix();
    
    // Underwater fog effect - gradient from dark at edges
    glBegin(GL_QUADS);
    for (int angle = 0; angle < 360; angle += 30) {
        float rad = angle * 3.14159f / 180.0f;
        float x1 = cosf(rad) * 150.0f;
        float z1 = sinf(rad) * 150.0f;
        float x2 = cosf(rad + 0.524f) * 150.0f;
        float z2 = sinf(rad + 0.524f) * 150.0f;
        
        // Outer edge - darker, fading to black
        glColor4f(0.0f, 0.05f, 0.15f, 0.7f);
        glVertex3f(x1, 30.0f, z1);
        glVertex3f(x2, 30.0f, z2);
        
        // Inner - visible
        glColor4f(0.0f, 0.1f, 0.25f, 0.3f);
        glVertex3f(x2 * 0.7f, 30.0f, z2 * 0.7f);
        glVertex3f(x1 * 0.7f, 30.0f, z1 * 0.7f);
    }
    glEnd();
    
    // Bottom gradient - deeper darkness
    glBegin(GL_QUADS);
    glColor4f(0.0f, 0.04f, 0.1f, 1.0f);
    glVertex3f(-100.0f, -35.0f, -100.0f);
    glVertex3f(100.0f, -35.0f, -100.0f);
    glVertex3f(100.0f, -35.0f, 100.0f);
    glVertex3f(-100.0f, -35.0f, 100.0f);
    glEnd();
    
    glEnable(GL_LIGHTING);
}

// Draw a realistic fish body with bright red and yellow coloring
void drawFishBody(float scale, float tailWave) {
    glPushMatrix();
    
    // MAIN BODY - ellipsoid shape (head to tail)
    glPushMatrix();
    glScalef(2.2f * scale, 1.0f * scale, 0.8f * scale);
    // Bright red body base
    glColor4f(0.95f, 0.2f, 0.1f, 0.92f);
    glutSolidSphere(0.6f, 40, 40);
    
    // Yellow patches on body (front)
    glPushMatrix();
    glTranslatef(0.3f, 0.2f, 0.0f);
    glScalef(0.6f, 0.5f, 0.7f);
    glColor4f(1.0f, 0.85f, 0.0f, 0.9f);
    glutSolidSphere(0.4f, 30, 30);
    glPopMatrix();
    
    // Yellow patch on top (mid-body)
    glPushMatrix();
    glTranslatef(-0.1f, 0.4f, 0.0f);
    glScalef(0.7f, 0.5f, 0.6f);
    glColor4f(1.0f, 0.9f, 0.1f, 0.88f);
    glutSolidSphere(0.35f, 30, 30);
    glPopMatrix();
    
    glPopMatrix();
    
    // HEAD - rounded with red coloring
    glPushMatrix();
    glTranslatef(1.4f * scale, 0.0f, 0.0f);
    glScalef(1.2f * scale, 0.95f * scale, 0.85f * scale);
    glColor4f(0.92f, 0.25f, 0.15f, 0.93f);
    glutSolidSphere(0.5f, 35, 35);
    
    // Yellow mark on head
    glPushMatrix();
    glTranslatef(0.2f, -0.1f, 0.0f);
    glScalef(0.6f, 0.5f, 0.7f);
    glColor4f(1.0f, 0.8f, 0.0f, 0.87f);
    glutSolidSphere(0.3f, 25, 25);
    glPopMatrix();
    glPopMatrix();
    
    // SNOUT/MOUTH - bright yellow
    glPushMatrix();
    glTranslatef(1.95f * scale, 0.0f, 0.0f);
    glScalef(0.6f * scale, 0.5f * scale, 0.5f * scale);
    glColor4f(1.0f, 0.9f, 0.05f, 0.88f);
    glutSolidSphere(0.35f, 30, 30);
    glPopMatrix();
    
    // EYES - dark
    glPushMatrix();
    glTranslatef(1.5f * scale, 0.35f * scale, 0.5f * scale);
    glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
    glutSolidSphere(0.12f * scale, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.5f * scale, 0.35f * scale, -0.5f * scale);
    glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
    glutSolidSphere(0.12f * scale, 20, 20);
    glPopMatrix();
    
    // EYE HIGHLIGHTS - shiny reflection
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(1.55f * scale, 0.38f * scale, 0.52f * scale);
    glColor4f(0.9f, 0.9f, 0.95f, 0.8f);
    glutSolidSphere(0.04f * scale, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.55f * scale, 0.38f * scale, -0.52f * scale);
    glColor4f(0.9f, 0.9f, 0.95f, 0.8f);
    glutSolidSphere(0.04f * scale, 10, 10);
    glPopMatrix();
    glEnable(GL_LIGHTING);
    
    // DORSAL FIN (top) - red with yellow tips
    glPushMatrix();
    glTranslatef(0.2f * scale, 0.85f * scale, 0.0f);
    glColor4f(0.85f, 0.15f, 0.05f, 0.8f);
    
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.25f * scale, 0.8f * scale, 0.0f);
    glVertex3f(0.35f * scale, 0.6f * scale, 0.0f);
    glEnd();
    
    // Yellow tip
    glColor4f(0.98f, 0.85f, 0.0f, 0.75f);
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.25f * scale, 0.75f * scale, 0.0f);
    glVertex3f(-0.25f * scale, 0.8f * scale, 0.0f);
    glVertex3f(-0.15f * scale, 0.78f * scale, 0.0f);
    glEnd();
    glPopMatrix();
    
    // ANAL FIN (bottom) - yellow/red mix
    glPushMatrix();
    glTranslatef(-0.3f * scale, -0.65f * scale, 0.0f);
    glColor4f(0.9f, 0.4f, 0.1f, 0.75f);
    
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.2f * scale, -0.6f * scale, 0.0f);
    glVertex3f(0.25f * scale, -0.5f * scale, 0.0f);
    glEnd();
    glPopMatrix();
    
    // PECTORAL FIN - RIGHT (red)
    glPushMatrix();
    glTranslatef(0.5f * scale, -0.3f * scale, 0.8f * scale);
    glRotatef(sinf(globalTime * 3.0f + 1.5f) * 30.0f, 1.0f, 0.0f, 0.0f);
    glColor4f(0.88f, 0.3f, 0.1f, 0.7f);
    
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.15f * scale, 0.4f * scale, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.5f * scale);
    glEnd();
    glPopMatrix();
    
    // PECTORAL FIN - LEFT (red)
    glPushMatrix();
    glTranslatef(0.5f * scale, -0.3f * scale, -0.8f * scale);
    glRotatef(sinf(globalTime * 3.0f + 1.5f) * 30.0f, 1.0f, 0.0f, 0.0f);
    glColor4f(0.88f, 0.3f, 0.1f, 0.7f);
    
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.15f * scale, 0.4f * scale, 0.0f);
    glVertex3f(0.0f, 0.0f, -0.5f * scale);
    glEnd();
    glPopMatrix();
    
    // TAIL FIN - forked red with yellow edges
    glPushMatrix();
    glTranslatef(-1.6f * scale, 0.0f, 0.0f);
    glRotatef(tailWave * 35.0f, 0.0f, 1.0f, 0.0f);
    
    // Upper tail lobe - red
    glColor4f(0.85f, 0.2f, 0.1f, 0.85f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.8f * scale, 0.5f * scale, 0.0f);
    glVertex3f(-0.6f * scale, 0.35f * scale, 0.3f * scale);
    glEnd();
    
    // Lower tail lobe - red
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.8f * scale, -0.5f * scale, 0.0f);
    glVertex3f(-0.6f * scale, -0.35f * scale, 0.3f * scale);
    glEnd();
    
    // Tail back sections - with yellow edge
    glColor4f(0.95f, 0.75f, 0.05f, 0.8f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.8f * scale, 0.5f * scale, 0.0f);
    glVertex3f(-0.6f * scale, 0.35f * scale, -0.3f * scale);
    glEnd();
    
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.8f * scale, -0.5f * scale, 0.0f);
    glVertex3f(-0.6f * scale, -0.35f * scale, -0.3f * scale);
    glEnd();
    
    glPopMatrix();
    
    glPopMatrix();
}

// Draw single fish at position with animation
void drawFish(float x, float y, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // Tail wave animation
    float tailWave = sinf(globalTime * 5.5f);
    
    // Swimming undulation (up-down motion)
    float swimWave = sinf(globalTime * 2.0f) * 0.15f;
    glTranslatef(0.0f, swimWave, 0.0f);
    
    drawFishBody(scale, tailWave);
    
    glPopMatrix();
}

// Draw all fish
void drawAllFish() {
    for (size_t i = 0; i < fishes.size(); i++) {
        Fish& f = fishes[i];
        
        // Update fish position - larger range for seamless ocean
        if (f.direction == 0.0f) {
            // Swimming right
            f.x += f.speed * 0.008f;
            if (f.x > 50.0f) f.x = -50.0f;
        } else {
            // Swimming left
            f.x -= f.speed * 0.008f;
            if (f.x < -50.0f) f.x = 50.0f;
        }
        
        // Vertical movement
        f.y += sinf(globalTime * 1.5f + i) * 0.003f;
        if (f.y > 15.0f) f.y = 15.0f;
        if (f.y < -15.0f) f.y = -15.0f;
        
        // Z movement (side to side)
        f.z += cosf(globalTime * 1.2f + i) * 0.003f;
        if (f.z > 50.0f) f.z = -50.0f;
        if (f.z < -50.0f) f.z = 50.0f;
        
        glPushMatrix();
        
        // Rotate fish based on direction
        glTranslatef(f.x, f.y, f.z);
        if (f.direction != 0.0f) {
            glRotatef(f.direction, 0.0f, 1.0f, 0.0f);
        }
        
        // Draw with size variation
        float scale = 0.6f + i * 0.15f;
        glTranslatef(0.0f, 0.0f, 0.0f);
        
        // Tail wave for this fish (offset by fish index)
        float tailWave = sinf(globalTime * 5.5f + i * 0.8f);
        
        // Swimming undulation
        float swimWave = sinf(globalTime * 2.0f + i * 0.5f) * 0.15f;
        glTranslatef(0.0f, swimWave, 0.0f);
        
        drawFishBody(scale, tailWave);
        
        glPopMatrix();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Dynamic camera - smooth orbit around fish, staying within the ocean sphere
    float camAngle = globalTime * 0.15f;  // Slower rotation for smooth movement
    float camDist = 12.0f;
    float camHeight = sinf(globalTime * 0.3f) * 2.0f;  // Gentle vertical motion
    
    float camX = cosf(camAngle) * camDist;
    float camZ = sinf(camAngle) * camDist;
    float camY = 2.0f + camHeight;
    
    // Look at center with slight depth variation for dynamic feel
    float lookY = sinf(globalTime * 0.2f) * 0.5f;
    
    gluLookAt(
        camX, camY, camZ,           // Camera position
        0.0f, lookY, 0.0f,          // Look at center (fish school)
        0.0f, 1.0f, 0.0f            // Up vector
    );
    
    // Draw ocean environment (seamless sphere - no edges)
    drawUnderwater();
    
    // Draw water effects - light rays, caustics, flowing currents
    drawParticles();
    
    // Draw all fish swimming naturally
    drawAllFish();
    
    // Draw HUD text - minimal, clean
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Simple info
   
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
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
        const char* titleText = "Chapter 11: Ordovician-Silurian - Complex Marine Life";
        for (const char* c = titleText; *c; c++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
    }
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    // Deep ocean blue background
    glClearColor(0.0f, 0.08f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Light setup - simulates filtered sunlight through water
    GLfloat light_position[] = { 3.0f, 6.0f, 4.0f, 0.0f };
    GLfloat light_ambient[] = { 0.2f, 0.3f, 0.4f, 1.0f };
    GLfloat light_diffuse[] = { 0.6f, 0.7f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 0.7f, 0.8f, 0.9f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Material properties for shiny fish scales
    GLfloat material_specular[] = { 0.95f, 0.95f, 0.95f, 1.0f };
    GLfloat material_shininess[] = { 80.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
    
    initializeParticles();
    initializeFishes();
}

// Keyboard handler
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {  // ESC
        exit(0);
    }
}

void timer(int value) {
    globalTime += 0.016f;  // ~60 FPS
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Chapter 11: Early Fish Evolution - Realistic 3D Simulation");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glutMainLoop();
    
    return 0;
}
