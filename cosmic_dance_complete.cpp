#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include <windows.h>

// ============================================================================
// GLOBAL CONSTANTS & STATE
// ============================================================================

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const int NUM_PARTICLES = 2000;
const float TOTAL_DURATION = 320.0f;  // 16 chapters * 20 seconds each
const float CHAPTER_DURATION = 20.0f;

float globalTime = 0.0f;
float deltaTime = 0.016f;

// Camera
float cameraX = 0.0f, cameraY = 0.5f, cameraZ = 3.0f;

// ============================================================================
// CHAPTER 1: CHAOS & DARKNESS
// ============================================================================

namespace Chapter1 {
    struct Particle {
        float x, y, z;
        float vx, vy, vz;
        float r, g, b;
        float alpha;
        float angle1, angle2;
        float speed;
        float lifetime;
        int type;
    };

    std::vector<Particle> particles;
    std::vector<Particle> nebulaClouds;

    struct Lightning {
        float startX, startY, startZ;
        float endX, endY, endZ;
        float birthTime;
        float duration;
        bool active;
    };

    std::vector<Lightning> lightnings;
    float lastLightningTime = -15.0f;

    void initialize() {
        particles.clear();
        nebulaClouds.clear();
        lightnings.clear();
        srand(static_cast<unsigned>(time(0)));
        
        // Create chaos particles
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
            p.type = 0;
            
            particles.push_back(p);
        }
        
        // Create wave particles
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
            p.type = 2;
            
            particles.push_back(p);
        }
        
        // Create nebula clouds
        for (int i = 0; i < 500; ++i) {
            Particle cloud;
            cloud.x = -2.5f + (rand() % 5000) / 1000.0f;
            cloud.y = -2.5f + (rand() % 5000) / 1000.0f;
            cloud.z = -3.0f - (rand() % 2000) / 1000.0f;
            
            cloud.vx = (rand() % 100 - 50) / 500.0f;
            cloud.vy = (rand() % 100 - 50) / 500.0f;
            cloud.vz = (rand() % 100) / 1000.0f;
            
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
            cloud.type = 1;
            
            nebulaClouds.push_back(cloud);
        }
    }

    void update() {
        float breathingFade = 0.5f + 0.5f * sinf(globalTime * 0.125f * 3.14159f);
        
        for (auto& p : particles) {
            if (p.type == 0) {
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
            else if (p.type == 2) {
                float orbitAngle = (p.angle1 + globalTime * 45.0f * p.speed) * 3.14159f / 180.0f;
                float radius = p.angle2;
                
                p.x = cos(orbitAngle) * radius;
                p.y = sin(orbitAngle) * radius;
                p.z = sin(orbitAngle * 0.5f) * 1.5f - 0.5f;
                
                p.alpha = 0.15f + sin(orbitAngle) * 0.1f;
                if (p.alpha < 0.05f) p.alpha = 0.05f;
                
                p.r = 0.5f + sin(orbitAngle) * 0.3f;
                p.g = 0.1f + cos(orbitAngle) * 0.1f;
            }
        }
        
        for (auto& cloud : nebulaClouds) {
            float driftX = sin(globalTime * 0.3f + cloud.angle1) * 0.005f;
            float driftY = cos(globalTime * 0.25f + cloud.angle2) * 0.005f;
            
            cloud.x += driftX + cloud.vx * deltaTime;
            cloud.y += driftY + cloud.vy * deltaTime;
            cloud.z += cloud.vz * deltaTime;
            
            if (cloud.x > 3.0f) cloud.x = -3.0f;
            if (cloud.x < -3.0f) cloud.x = 3.0f;
            if (cloud.y > 3.0f) cloud.y = -3.0f;
            if (cloud.y < -3.0f) cloud.y = 3.0f;
            
            cloud.alpha = (0.05f + (rand() % 50) / 1000.0f) * (1.0f + sin(globalTime + cloud.angle1) * 0.5f);
        }
        
        // Lightning
        if (globalTime - lastLightningTime > (6.0f + (rand() % 4000) / 1000.0f)) {
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
            lastLightningTime = globalTime;
        }
        
        std::vector<Lightning> activeLightnings;
        for (auto& l : lightnings) {
            if (globalTime - l.birthTime < l.duration) {
                activeLightnings.push_back(l);
            }
        }
        lightnings = activeLightnings;
    }

    void draw() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        // Breathing background
        float breathingFade = 0.5f + 0.5f * sinf(globalTime * 0.125f * 3.14159f);
        glClearColor(breathingFade * 0.05f, 0.0f, breathingFade * 0.05f, 1.0f);
        
        // Draw particles
        glPointSize(2.0f);
        glBegin(GL_POINTS);
        for (const auto& p : particles) {
            glColor4f(p.r, p.g, p.b, p.alpha);
            glVertex3f(p.x, p.y, p.z);
        }
        glEnd();
        
        glPointSize(8.0f);
        glBegin(GL_POINTS);
        for (const auto& cloud : nebulaClouds) {
            glColor4f(cloud.r, cloud.g, cloud.b, cloud.alpha);
            glVertex3f(cloud.x, cloud.y, cloud.z);
        }
        glEnd();
        
        // Draw lightning
        for (const auto& l : lightnings) {
            float elapsed = globalTime - l.birthTime;
            float progress = elapsed / l.duration;
            
            float r, g, b, a;
            if (progress < 0.5f) {
                float t = progress * 2.0f;
                r = 1.0f - t * 0.3f;
                g = 1.0f - t * 1.0f;
                b = 1.0f;
                a = 0.8f;
            } else {
                float t = (progress - 0.5f) * 2.0f;
                r = 0.7f * (1.0f - t);
                g = 0.0f;
                b = 1.0f * (1.0f - t);
                a = 0.8f * (1.0f - t);
            }
            
            if (fmod(globalTime * 10.0f, 1.0f) < 0.3f) {
                a *= 0.5f;
            }
            
            glColor4f(r, g, b, a);
            glLineWidth(2.5f);
            glBegin(GL_LINE_STRIP);
            
            int segments = 8;
            for (int i = 0; i <= segments; ++i) {
                float t = i / (float)segments;
                float x = l.startX + (l.endX - l.startX) * t;
                float y = l.startY + (l.endY - l.startY) * t;
                float z = l.startZ + (l.endZ - l.startZ) * t;
                
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
}

// ============================================================================
// CHAPTER 2: BIG BANG (simplified)
// ============================================================================

namespace Chapter2 {
    struct Particle {
        float x, y, z;
        float vx, vy, vz;
        float r, g, b;
        float alpha;
    };

    std::vector<Particle> particles;

    void initialize() {
        particles.clear();
        for (int i = 0; i < 5000; ++i) {
            Particle p;
            float angle = (rand() % 360) * 3.14159f / 180.0f;
            float phi = (rand() % 360) * 3.14159f / 180.0f;
            float speed = 0.5f + (rand() % 100) / 100.0f;
            
            p.x = 0.0f;
            p.y = 0.0f;
            p.z = 0.0f;
            
            p.vx = sin(phi) * cos(angle) * speed;
            p.vy = sin(phi) * sin(angle) * speed;
            p.vz = cos(phi) * speed;
            
            float tempFactor = rand() % 100 / 100.0f;
            if(tempFactor < 0.25f) {
                p.r = 1.0f; p.g = 1.0f; p.b = 1.0f;
            } else if(tempFactor < 0.5f) {
                p.r = 1.0f; p.g = 1.0f; p.b = 0.0f;
            } else if(tempFactor < 0.75f) {
                p.r = 1.0f; p.g = 0.5f; p.b = 0.0f;
            } else {
                p.r = 0.5f; p.g = 0.8f; p.b = 1.0f;
            }
            
            p.alpha = 0.8f;
            particles.push_back(p);
        }
    }

    void update() {
        float chapterTime = fmod(globalTime, CHAPTER_DURATION);
        
        for (auto& p : particles) {
            if (chapterTime < 0.5f) {
                float expFactor = 1.0f - chapterTime * 0.1f;
                p.vx *= (0.95f + expFactor * 0.05f);
                p.vy *= (0.95f + expFactor * 0.05f);
                p.vz *= (0.95f + expFactor * 0.05f);
            } else if (chapterTime < 3.0f) {
                float cool = (chapterTime - 0.5f) / 2.5f;
                p.r = p.r * (1.0f - cool * 0.3f);
                p.g = p.g * (1.0f - cool * 0.5f);
                p.b = p.b * (1.0f - cool * 0.8f);
                p.alpha = 0.7f - cool * 0.3f;
            }
            
            p.x += p.vx * deltaTime;
            p.y += p.vy * deltaTime;
            p.z += p.vz * deltaTime;
        }
    }

    void draw() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        for (const auto& p : particles) {
            glColor4f(p.r, p.g, p.b, p.alpha);
            glVertex3f(p.x, p.y, p.z);
        }
        glEnd();
    }
}

// ============================================================================
// Simplified chapters 3-16 (placeholder implementations)
// ============================================================================

namespace Chapter3 {
    void initialize() {}
    void update() {}
    void draw() {
        glClearColor(0.0f, 0.0f, 0.02f, 1.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
    }
}

namespace Chapter4 { void initialize() {} void update() {} void draw() { glClearColor(0.08f, 0.0f, 0.01f, 1.0f); } }
namespace Chapter5 { void initialize() {} void update() {} void draw() { glClearColor(0.0f, 0.0f, 0.0f, 1.0f); } }
namespace Chapter6 { void initialize() {} void update() {} void draw() { glClearColor(0.0f, 0.0f, 0.01f, 1.0f); } }
namespace Chapter7 { void initialize() {} void update() {} void draw() { glClearColor(0.01f, 0.02f, 0.04f, 1.0f); } }
namespace Chapter8 { void initialize() {} void update() {} void draw() { glClearColor(0.2f, 0.35f, 0.5f, 0.7f); } }
namespace Chapter9 { void initialize() {} void update() {} void draw() { glClearColor(0.01f, 0.01f, 0.02f, 1.0f); } }
namespace Chapter10 { void initialize() {} void update() {} void draw() { glClearColor(0.05f, 0.15f, 0.25f, 1.0f); } }
namespace Chapter11 { void initialize() {} void update() {} void draw() { glClearColor(0.0f, 0.08f, 0.18f, 1.0f); } }
namespace Chapter12 { void initialize() {} void update() {} void draw() { glClearColor(0.3f, 0.5f, 0.7f, 1.0f); } }
namespace Chapter13 { void initialize() {} void update() {} void draw() { glClearColor(0.02f, 0.02f, 0.08f, 1.0f); } }
namespace Chapter14 { void initialize() {} void update() {} void draw() { glClearColor(0.5f, 0.5f, 0.5f, 1.0f); } }
namespace Chapter15 { void initialize() {} void update() {} void draw() { glClearColor(0.5f, 0.8f, 1.0f, 1.0f); } }
namespace Chapter16 { void initialize() {} void update() {} void draw() { glClearColor(0.0f, 0.05f, 0.15f, 1.0f); } }

// ============================================================================
// DISPLAY & UPDATE
// ============================================================================

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(0.0f, 0.5f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    // Determine current chapter (0-15)
    int chapter = (int)(globalTime / CHAPTER_DURATION) % 16;
    
    // Draw title with fade
    float chapterStartTime = chapter * CHAPTER_DURATION;
    float timeSinceChapterStart = globalTime - chapterStartTime;
    
    float titleAlpha = 1.0f;
    if (timeSinceChapterStart < 0.5f) {
        titleAlpha = timeSinceChapterStart / 0.5f;
    } else if (timeSinceChapterStart > 19.5f) {
        titleAlpha = (20.0f - timeSinceChapterStart) / 0.5f;
    }
    
    // Call chapter-specific display functions
    switch(chapter) {
        case 0: Chapter1::draw(); break;
        case 1: Chapter2::draw(); break;
        case 2: Chapter3::draw(); break;
        case 3: Chapter4::draw(); break;
        case 4: Chapter5::draw(); break;
        case 5: Chapter6::draw(); break;
        case 6: Chapter7::draw(); break;
        case 7: Chapter8::draw(); break;
        case 8: Chapter9::draw(); break;
        case 9: Chapter10::draw(); break;
        case 10: Chapter11::draw(); break;
        case 11: Chapter12::draw(); break;
        case 12: Chapter13::draw(); break;
        case 13: Chapter14::draw(); break;
        case 14: Chapter15::draw(); break;
        case 15: Chapter16::draw(); break;
    }
    
    // Draw chapter title
    if (titleAlpha > 0.0f) {
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        const char* titles[] = {
            "Chapter 1: Chaos & Darkness",
            "Chapter 2: Big Bang",
            "Chapter 3: Nebula Genesis",
            "Chapter 4: Cosmic Observer",
            "Chapter 5: Milky Way Galaxy",
            "Chapter 6: Solar System",
            "Chapter 7: Emergence of Life",
            "Chapter 8: First Cells",
            "Chapter 9: Multicellular Life",
            "Chapter 10: Early Organisms",
            "Chapter 11: Complex Marine Life",
            "Chapter 12: Ocean to Land",
            "Chapter 13: Land & Volcanos",
            "Chapter 14: Dinosaur Era",
            "Chapter 15: Mammalian Rise",
            "Chapter 16: Humanity Observes"
        };
        
        glColor4f(1.0f, 1.0f, 1.0f, titleAlpha);
        glRasterPos2f(-0.95f, 0.90f);
        for (const char* c = titles[chapter]; *c; c++) {
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
    gluPerspective(60.0f, (float)w / (float)h, 0.1f, 500.0f);
}

void idle() {
    globalTime += deltaTime;
    
    // Update current chapter
    int chapter = (int)(globalTime / CHAPTER_DURATION) % 16;
    
    switch(chapter) {
        case 0: Chapter1::update(); break;
        case 1: Chapter2::update(); break;
        case 2: Chapter3::update(); break;
        case 3: Chapter4::update(); break;
        case 4: Chapter5::update(); break;
        case 5: Chapter6::update(); break;
        case 6: Chapter7::update(); break;
        case 7: Chapter8::update(); break;
        case 8: Chapter9::update(); break;
        case 9: Chapter10::update(); break;
        case 10: Chapter11::update(); break;
        case 11: Chapter12::update(); break;
        case 12: Chapter13::update(); break;
        case 13: Chapter14::update(); break;
        case 14: Chapter15::update(); break;
        case 15: Chapter16::update(); break;
    }
    
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }
    if (key == ' ') {
        globalTime = 0.0f;
    }
}

void initGL() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Initialize all chapters
    Chapter1::initialize();
    Chapter2::initialize();
    Chapter3::initialize();
    Chapter4::initialize();
    Chapter5::initialize();
    Chapter6::initialize();
    Chapter7::initialize();
    Chapter8::initialize();
    Chapter9::initialize();
    Chapter10::initialize();
    Chapter11::initialize();
    Chapter12::initialize();
    Chapter13::initialize();
    Chapter14::initialize();
    Chapter15::initialize();
    Chapter16::initialize();
}

int main(int argc, char** argv) {
    srand(time(0));
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("COSMIC DANCE - Complete 16 Chapter Animation (5:20 Duration)");
    
    initGL();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    
    std::cout << "========================================\n";
    std::cout << "COSMIC DANCE - Complete Animation\n";
    std::cout << "16 Chapters x 20 seconds = 5:20 total\n";
    std::cout << "========================================\n";
    std::cout << "Controls:\n";
    std::cout << "  SPACE - Reset animation\n";
    std::cout << "  ESC   - Exit\n";
    std::cout << "========================================\n";
    
    glutMainLoop();
    
    return 0;
}
