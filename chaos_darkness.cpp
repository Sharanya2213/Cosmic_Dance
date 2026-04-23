#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

// ============================================================================
// CHAPTER 1: CHAOS AND DARKNESS (0–12s)
// Subtitle: "In the beginning... nothing. Then everything."
// ============================================================================

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const int NUM_PARTICLES = 5000;
const float CHAPTER_DURATION = 12.0f;

float elapsed = 0.0f;       // Global elapsed time in seconds
float deltaTime = 0.016f;   // ~60 FPS

// ============================================================================
// PARTICLE STRUCTURE
// ============================================================================

struct Particle {
    float x, y, z;        // Position
    float vx, vy, vz;     // Velocity
    float r, g, b, a;     // Color + alpha
    float size;           // Point size
    float life;           // 0.0 to 1.0
    float decay;          // How fast life decreases
    float wobbleFreq;     // Jitter frequency
    float wobbleAmp;      // Jitter amplitude
};

std::vector<Particle> particles;

// ============================================================================
// INITIALIZATION
// ============================================================================

void initParticles() {
    particles.clear();
    srand(static_cast<unsigned>(time(0)));
    
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        Particle p;
        
        // Random position in 400-unit sphere
        float theta = (rand() % 360) * 3.14159f / 180.0f;
        float phi = (rand() % 360) * 3.14159f / 180.0f;
        float r_sphere = (rand() % 400) / 400.0f * 200.0f;  // max 200 units radius
        
        p.x = r_sphere * sinf(phi) * cosf(theta);
        p.y = r_sphere * sinf(phi) * sinf(theta);
        p.z = r_sphere * cosf(phi);
        
        // Initial velocity (small, chaotic)
        p.vx = (rand() % 100 - 50) / 500.0f;
        p.vy = (rand() % 100 - 50) / 500.0f;
        p.vz = (rand() % 100 - 50) / 500.0f;
        
        // Colors: deep red, dark violet, near-black
        int colorType = rand() % 3;
        if (colorType == 0) {
            // Deep red
            p.r = 0.6f + (rand() % 40) / 100.0f;
            p.g = 0.0f;
            p.b = 0.1f + (rand() % 10) / 100.0f;
        } else if (colorType == 1) {
            // Dark violet
            p.r = 0.4f + (rand() % 30) / 100.0f;
            p.g = 0.0f;
            p.b = 0.4f + (rand() % 30) / 100.0f;
        } else {
            // Near-black
            p.r = (rand() % 20) / 100.0f;
            p.g = (rand() % 20) / 100.0f;
            p.b = (rand() % 20) / 100.0f;
        }
        
        p.a = 0.1f + (rand() % 50) / 100.0f;
        p.size = 1.0f + (rand() % 20) / 10.0f;
        p.life = 1.0f;
        p.decay = 0.01f + (rand() % 20) / 1000.0f;
        
        // Jitter parameters
        p.wobbleFreq = 2.0f + (rand() % 50) / 10.0f;
        p.wobbleAmp = 0.1f + (rand() % 30) / 100.0f;
        
        particles.push_back(p);
    }
}

// ============================================================================
// UPDATE
// ============================================================================

void updateParticles() {
    float progress = elapsed / CHAPTER_DURATION;  // 0 to 1
    
    for (auto& p : particles) {
        // Unstable jittering motion
        float jitterX = sinf(elapsed * p.wobbleFreq + p.x) * p.wobbleAmp;
        float jitterY = sinf(elapsed * p.wobbleFreq * 0.7f + p.y) * p.wobbleAmp;
        float jitterZ = sinf(elapsed * p.wobbleFreq * 1.3f + p.z) * p.wobbleAmp;
        
        // Apply jitter
        p.x += jitterX * deltaTime;
        p.y += jitterY * deltaTime;
        p.z += jitterZ * deltaTime;
        
        // Slow drift toward center (first hint of order)
        float centerPull = progress * 0.02f;  // Increases over chapter
        float distFromCenter = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
        
        if (distFromCenter > 0.1f) {
            p.x -= (p.x / distFromCenter) * centerPull;
            p.y -= (p.y / distFromCenter) * centerPull;
            p.z -= (p.z / distFromCenter) * centerPull;
        }
        
        // Add some chaos velocity
        p.vx += (rand() % 100 - 50) / 10000.0f;
        p.vy += (rand() % 100 - 50) / 10000.0f;
        p.vz += (rand() % 100 - 50) / 10000.0f;
        
        // Limit velocity
        float speed = sqrtf(p.vx * p.vx + p.vy * p.vy + p.vz * p.vz);
        if (speed > 1.0f) {
            p.vx *= 0.9f / speed;
            p.vy *= 0.9f / speed;
            p.vz *= 0.9f / speed;
        }
        
        // Apply velocity
        p.x += p.vx * deltaTime;
        p.y += p.vy * deltaTime;
        p.z += p.vz * deltaTime;
        
        // Random energy surges (bright flashes)
        if ((rand() % 1000) < 5) {  // 0.5% chance
            p.r = 1.0f;
            p.g = 1.0f;
            p.b = 1.0f;
            p.a = 0.8f;
            p.size = 3.0f;
        } else {
            // Fade back
            if (p.r > 0.7f) p.r -= 0.1f;
            if (p.g > 0.5f) p.g -= 0.05f;
            if (p.b > 0.5f) p.b -= 0.05f;
            if (p.size > 1.5f) p.size -= 0.1f;
        }
    }
}

// ============================================================================
// RENDERING
// ============================================================================

void updateBackground() {
    // Screen flicker with dim irregular pulses
    float baseDarkness = 0.0f;
    if ((int)(elapsed * 10) % 3 == 0 && fmodf(elapsed * 10, 1.0f) < 0.1f) {
        baseDarkness = 0.05f;  // Dim flicker
    }
    glClearColor(baseDarkness, baseDarkness, baseDarkness, 1.0f);
}

void drawParticles() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // Additive blending for glow
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glDisable(GL_LIGHTING);
    
    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        glPointSize(p.size);
        glColor4f(p.r, p.g, p.b, p.a);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    
    glEnable(GL_LIGHTING);
    glDisable(GL_POINT_SMOOTH);
}

void drawText(const char* text, float x, float y, float r, float g, float b, float a, void* font) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor4f(r, g, b, a);
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(font, *text++);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void drawChapterTitle() {
    float alpha = 1.0f;
    
    // Fade in first 2 seconds
    if (elapsed < 2.0f) {
        alpha = elapsed / 2.0f;
    }
    // Fade out last 1 second
    else if (elapsed > CHAPTER_DURATION - 1.0f) {
        alpha = (CHAPTER_DURATION - elapsed) / 1.0f;
    }
    
    if (alpha > 0.01f) {
        drawText("CHAPTER 1: CHAOS AND DARKNESS",
                 (WINDOW_WIDTH - 550) / 2.0f, 50.0f,
                 1.0f, 1.0f, 1.0f, alpha,
                 GLUT_BITMAP_HELVETICA_18);
    }
}

void drawSubtitle() {
    float alpha = 1.0f;
    
    // Fade in first 2 seconds
    if (elapsed < 2.0f) {
        alpha = elapsed / 2.0f;
    }
    // Fade out last 1 second
    else if (elapsed > CHAPTER_DURATION - 1.0f) {
        alpha = (CHAPTER_DURATION - elapsed) / 1.0f;
    }
    
    if (alpha > 0.01f) {
        drawText("In the beginning... nothing. Then everything.",
                 (WINDOW_WIDTH - 450) / 2.0f, WINDOW_HEIGHT - 60.0f,
                 0.9f, 0.9f, 0.9f, alpha,
                 GLUT_BITMAP_HELVETICA_12);
    }
}

void display() {
    updateBackground();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Camera positioned far back
    gluLookAt(0, 0, 400.0f,      // Eye position
              0, 0, 0,            // Look at (center)
              0, 1, 0);           // Up vector
    
    // Draw particles
    drawParticles();
    
    // Draw UI text
    drawChapterTitle();
    drawSubtitle();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)w / (float)h, 0.1f, 5000.0f);
}

void idle() {
    elapsed += deltaTime;
    
    // Auto-advance chapter after duration
    if (elapsed > CHAPTER_DURATION) {
        std::cout << "Chapter 1 complete (12 seconds). Press ESC to exit." << std::endl;
        elapsed = CHAPTER_DURATION;
    }
    
    updateParticles();
    glutPostRedisplay();
}

void timer(int value) {
    idle();
    glutTimerFunc(16, timer, 0);  // ~60 FPS
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {  // ESC
        exit(0);
    } else if (key == ' ') {  // SPACE - reset
        elapsed = 0.0f;
        initParticles();
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    initParticles();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("THE COSMIC DANCE OF CREATION - Chapter 1: Chaos and Darkness");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);
    
    init();
    
    std::cout << "===== CHAPTER 1: CHAOS AND DARKNESS =====" << std::endl;
    std::cout << "Duration: 12 seconds" << std::endl;
    std::cout << "5000 jittering particles in primordial void" << std::endl;
    std::cout << "Controls: SPACE=reset, ESC=exit" << std::endl;
    
    glutMainLoop();
    return 0;
}
