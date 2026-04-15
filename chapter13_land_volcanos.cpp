#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdio>

float globalTime = 0.0f;

// ============================================================================
// STRUCTURES
// ============================================================================

struct Amphibian {
    float x, z;
    float dir;
    float speed;
};

struct Tree {
    float x, z;
    float height;
};

struct Volcano {
    float x, z;
    float radius;
    float height;
};

struct SmokeParticle {
    float x, y, z;
    float vx, vy, vz;
    float life;
    float age;
};

struct Star {
    float x, y, z;
    float brightness;
};

std::vector<Amphibian> amphibians;
std::vector<Tree> trees;
std::vector<Volcano> volcanoes;
std::vector<SmokeParticle> smoke;
std::vector<Star> stars;

// ============================================================================
// ISLAND HEIGHT MAP (CIRCULAR ISLAND)
// ============================================================================

float getIslandHeight(float x, float z) {
    float dist = sqrtf(x*x + z*z);
    
    // Island peaks in center, slopes to edges
    float islandShape = sinf(dist * 0.2f) * 0.5f;
    
    // Additional terrain variation
    float h = islandShape;
    h += sinf(x * 0.15f) * 0.2f;
    h += cosf(z * 0.18f) * 0.2f;
    
    // Smooth falloff at edges
    if (dist > 20.0f) {
        h *= (25.0f - dist) / 5.0f;
    }
    
    return h;
}

// ============================================================================
// INITIALIZATION
// ============================================================================

void initAmphibians() {
    amphibians.clear();
    for (int i = 0; i < 10; i++) {
        Amphibian a;
        a.x = (rand() % 300 - 150) / 10.0f;
        a.z = (rand() % 300 - 150) / 10.0f;
        a.dir = rand() % 360;
        a.speed = 0.008f + (rand() % 5) / 1000.0f;
        float dist = sqrtf(a.x*a.x + a.z*a.z);
        if (dist < 18.0f) amphibians.push_back(a);
    }
}

void initTrees() {
    trees.clear();
    srand(42);  // Fixed seed for consistent placement
    
    // Create DENSE forest of palm trees around island
    for (int angle = 0; angle < 360; angle += 10) {
        float ang_rad = angle * 3.14159f / 180.0f;
        for (float dist = 2.0f; dist < 18.0f; dist += 1.5f) {
            for (int i = 0; i < 2; i++) {
                float x = cosf(ang_rad) * dist + (rand() % 40 - 20) / 10.0f;
                float z = sinf(ang_rad) * dist + (rand() % 40 - 20) / 10.0f;
                float check_dist = sqrtf(x*x + z*z);
                
                if (check_dist < 19.0f) {
                    Tree t;
                    t.x = x;
                    t.z = z;
                    t.height = 5.0f + (rand() % 40) / 100.0f;  // Taller: 5.0-5.4 units (palm trees)
                    trees.push_back(t);
                }
            }
        }
    }
    printf("Initialized %d trees\n", (int)trees.size());
}

void initVolcanoes() {
    volcanoes.clear();
    // Single prominent volcano at island EDGE - opposite side
    volcanoes.push_back({-14.0f, -14.0f, 4.0f, 14.0f});
}

void initStars() {
    stars.clear();
    srand(123);
    // Create ~200 stars scattered across the sky
    for (int i = 0; i < 200; i++) {
        Star s;
        s.x = (rand() % 2000 - 1000) / 100.0f;  // -10 to 10 in x
        s.y = (rand() % 1000 + 500) / 100.0f;   // 5 to 15 in y (high up)
        s.z = (rand() % 2000 - 1000) / 100.0f;  // -10 to 10 in z
        s.brightness = 0.3f + (rand() % 70) / 100.0f;  // 0.3 to 1.0
        stars.push_back(s);
    }
}

// ============================================================================
// DRAWING FUNCTIONS
// ============================================================================

void drawTerrain() {
    int gridSize = 40;
    float cellSize = 60.0f / gridSize;
    
    for (int ix = 0; ix < gridSize; ix++) {
        glBegin(GL_QUAD_STRIP);
        for (int iz = 0; iz <= gridSize; iz++) {
            float x1 = -30.0f + ix * cellSize;
            float z = -30.0f + iz * cellSize;
            float y1 = getIslandHeight(x1, z);
            
            float x2 = -30.0f + (ix + 1) * cellSize;
            float y2 = getIslandHeight(x2, z);
            
            // Distance-based coloring: green in center, sand at edges
            float dist1 = sqrtf(x1*x1 + z*z);
            float dist2 = sqrtf(x2*x2 + z*z);
            
            // Green land in center, sandy beach at edges (8-10 units from center)
            if (dist1 < 8.0f) {
                glColor3f(0.2f, 0.5f, 0.15f);  // Dark green
            } else if (dist1 < 10.0f) {
                // Transition to sand
                float sand_blend = (dist1 - 8.0f) / 2.0f;
                glColor3f(0.2f + sand_blend*0.55f, 0.5f - sand_blend*0.15f, 0.15f - sand_blend*0.1f);
            } else {
                glColor3f(0.75f, 0.65f, 0.3f);  // Sandy beach
            }
            
            glVertex3f(x1, y1, z);
            
            if (dist2 < 8.0f) {
                glColor3f(0.2f, 0.5f, 0.15f);
            } else if (dist2 < 10.0f) {
                float sand_blend = (dist2 - 8.0f) / 2.0f;
                glColor3f(0.2f + sand_blend*0.55f, 0.5f - sand_blend*0.15f, 0.15f - sand_blend*0.1f);
            } else {
                glColor3f(0.75f, 0.65f, 0.3f);
            }
            
            glVertex3f(x2, y2, z);
        }
        glEnd();
    }
}

void drawTree(const Tree& t) {
    float ty = getIslandHeight(t.x, t.z);
    
    glPushMatrix();
    glTranslatef(t.x, ty, t.z);
    
    // Trunk: VERY TALL slim cylinder (palm tree trunk)
    glColor3f(0.35f, 0.25f, 0.1f);
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  // Rotate to vertical
    gluCylinder(gluNewQuadric(), 0.12f, 0.10f, t.height, 6, 6);
    glPopMatrix();
    
    // Foliage: Palm tree fronds using spheres for leaves
    float trunkTop = t.height;
    
    // Large spray of leaves at top (palm fronds effect)
    glColor3f(0.25f, 0.65f, 0.2f);  // Vibrant green
    
    // Central cluster of foliage (large)
    glPushMatrix();
    glTranslatef(0.0f, trunkTop + 0.3f, 0.0f);
    glScalef(1.3f, 0.8f, 1.3f);
    glutSolidSphere(0.9f, 10, 8);
    glPopMatrix();
    
    // Side frond clusters (for palm effect)
    glColor3f(0.2f, 0.60f, 0.18f);
    for (int i = 0; i < 4; i++) {
        float angle = (i * 90.0f) * 3.14159f / 180.0f;
        float fx = cosf(angle) * 0.7f;
        float fz = sinf(angle) * 0.7f;
        
        glPushMatrix();
        glTranslatef(fx, trunkTop + 0.2f, fz);
        glScalef(0.7f, 0.6f, 0.7f);
        glutSolidSphere(0.6f, 8, 6);
        glPopMatrix();
    }
    
    // Top tip (bright green peak)
    glColor3f(0.35f, 0.75f, 0.25f);
    glPushMatrix();
    glTranslatef(0.0f, trunkTop + 1.2f, 0.0f);
    glutSolidSphere(0.35f, 8, 6);
    glPopMatrix();
    
    glPopMatrix();
}

void drawVolcano(const Volcano& v) {
    float vy = getIslandHeight(v.x, v.z);
    
    glPushMatrix();
    glTranslatef(v.x, vy, v.z);
    
    // Main volcano cone - VERTICAL (pointing up along Y axis)
    glColor3f(0.6f, 0.35f, 0.2f);  // Warm brown-red
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  // CRITICAL: Make cone vertical!
    glutSolidCone(v.radius, v.height, 20, 16);
    glPopMatrix();
    
    // Darker crater shadow at top
    glColor3f(0.4f, 0.25f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, v.height - 0.4f, 0.0f);
    glutSolidSphere(v.radius * 0.5f, 16, 8);
    glPopMatrix();
    
    // Crater glow - bright lava (at apex)
    float craterGlow = 0.5f + sinf(globalTime * 4.0f) * 0.4f;
    glColor4f(1.0f, 0.6f, 0.1f, craterGlow);
    glPushMatrix();
    glTranslatef(0.0f, v.height, 0.0f);
    glEnable(GL_BLEND);
    glutSolidSphere(v.radius * 0.8f, 12, 10);
    glDisable(GL_BLEND);
    glPopMatrix();
    
    glPopMatrix();
    
    // Spawn MORE smoke particles for dramatic effect
    if (rand() % 6 == 0) {
        for (int i = 0; i < 4; i++) {
            SmokeParticle p;
            p.x = v.x + (rand() % 30 - 15) / 20.0f;
            p.y = vy + v.height;
            p.z = v.z + (rand() % 30 - 15) / 20.0f;
            p.vx = (rand() % 20 - 10) / 250.0f;
            p.vy = 0.10f + (rand() % 15) / 300.0f;  // Faster upward
            p.vz = (rand() % 20 - 10) / 250.0f;
            p.life = 1.2f;
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
        it->x += it->vx;
        it->y += it->vy;
        it->z += it->vz;
        
        if (it->age >= it->life) {
            it = smoke.erase(it);
        } else {
            float alpha = 0.5f * (1.0f - it->age / it->life);  // More visible
            glColor4f(0.75f, 0.75f, 0.75f, alpha);  // Lighter smoke
            
            glPushMatrix();
            glTranslatef(it->x, it->y, it->z);
            float size = 0.25f + (it->age / it->life) * 0.8f;  // Larger clouds
            glScalef(size, size, size);
            glutSolidSphere(1.0f, 5, 5);
            glPopMatrix();
            
            ++it;
        }
    }
    glDisable(GL_BLEND);
}

void drawAmphibian(const Amphibian& a) {
    float ty = getIslandHeight(a.x, a.z);
    float hop = fabsf(sinf(globalTime * 3.0f)) * 0.08f;
    
    glPushMatrix();
    glTranslatef(a.x, ty + 0.15f + hop, a.z);
    
    // Body
    glColor3f(0.6f, 0.7f, 0.3f);
    glPushMatrix();
    glScalef(0.25f, 0.15f, 0.35f);
    glutSolidSphere(1.0f, 6, 5);
    glPopMatrix();
    
    // Head
    glColor3f(0.65f, 0.72f, 0.35f);
    glPushMatrix();
    glTranslatef(0.0f, 0.08f, 0.15f);
    glutSolidSphere(0.08f, 5, 4);
    glPopMatrix();
    
    // Eyes
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-0.05f, 0.1f, 0.19f);
    glutSolidSphere(0.02f, 4, 3);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.05f, 0.1f, 0.19f);
    glutSolidSphere(0.02f, 4, 3);
    glPopMatrix();
    
    glPopMatrix();
}

void renderText(const char* text, float x, float y) {
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);  // White text
    glRasterPos2f(x, y);
    
    // Render each character
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

void drawSky() {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glBegin(GL_QUADS);
    // Dark night sky - very dark blue to black
    glColor3f(0.02f, 0.02f, 0.08f);  // Almost black
    glVertex2f(0, 1);
    glVertex2f(1, 1);
    // Slightly lighter at horizon (dark blue)
    glColor3f(0.05f, 0.05f, 0.15f);
    glVertex2f(1, 0);
    glVertex2f(0, 0);
    glEnd();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void drawStars() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Draw each star with twinkling effect
    for (const auto& s : stars) {
        // Twinkle effect based on star position and global time
        float twinkle = 0.5f + 0.5f * sinf(globalTime * 2.0f + s.x * 0.5f + s.z * 0.3f);
        float brightness = s.brightness * twinkle;
        
        glColor3f(brightness, brightness, brightness * 0.95f);
        
        glPushMatrix();
        glTranslatef(s.x, s.y, s.z);
        glPointSize(2.0f);
        glBegin(GL_POINTS);
        glVertex3f(0, 0, 0);
        glEnd();
        glPopMatrix();
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void drawClouds() {
    // Disabled for night scene
    // (Optional: could draw dark silhouettes of clouds if desired)
}

void drawOcean() {
    // DARKER BLUE ocean
    glColor4f(0.0f, 0.15f, 0.35f, 0.85f);
    
    float step = 50.0f / 40.0f;  // 40×40 grid
    
    glEnable(GL_BLEND);
    for (float x = -25.0f; x < 25.0f; x += step) {
        glBegin(GL_QUAD_STRIP);
        for (float z = -20.0f; z <= 20.0f; z += step) {
            float dist_sq = x*x + z*z;
            if (dist_sq > 400.0f) {  // Only draw ocean outside island
                float wave1 = sinf(x * 0.4f + globalTime * 0.8f) * 0.3f;
                float wave2 = cosf(z * 0.3f + globalTime * 0.6f) * 0.25f;
                float y = -0.5f + wave1 + wave2;
                
                float wave1_2 = sinf((x + step) * 0.4f + globalTime * 0.8f) * 0.3f;
                float wave2_2 = cosf(z * 0.3f + globalTime * 0.6f) * 0.25f;
                float y2 = -0.5f + wave1_2 + wave2_2;
                
                glVertex3f(x, y, z);
                glVertex3f(x + step, y2, z);
            }
        }
        glEnd();
    }
    glDisable(GL_BLEND);
}

// ============================================================================
// UPDATE FUNCTIONS
// ============================================================================

void updateAmphibians() {
    for (auto& a : amphibians) {
        float dx = cosf(a.dir * 3.14159f / 180.0f) * a.speed;
        float dz = sinf(a.dir * 3.14159f / 180.0f) * a.speed;
        a.x += dx;
        a.z += dz;
        
        // Gentle random turning
        a.dir += sinf(globalTime * 0.3f + a.x * 0.02f) * 0.2f;
        
        // Boundary wrapping
        float dist = sqrtf(a.x * a.x + a.z * a.z);
        if (dist > 20.0f) {
            a.dir = atan2f(-a.z, -a.x) * 180.0f / 3.14159f;
        }
    }
}

// ============================================================================
// DISPLAY & CALLBACKS
// ============================================================================

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Draw sky FIRST
    drawSky();
    
    // Draw stars
    drawStars();
    
    // ZOOMED-IN camera view - much closer to the island
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(20, 15, 30,    0, 2, 0,    0, 1, 0);
    
    // Draw clouds
    drawClouds();
    
    // Draw environment
    drawTerrain();
    drawOcean();
    
    // Draw volcanoes - VERTICAL CONE
    for (const auto& v : volcanoes) {
        drawVolcano(v);
    }
    
    // Draw trees
    for (const auto& t : trees) {
        drawTree(t);
    }
    
    // Draw amphibians
    for (const auto& a : amphibians) {
        drawAmphibian(a);
    }
    
    // Draw smoke particles
    drawSmoke();
    
    // Draw text label
    renderText("Carboniferous Period", 50.0f, 540.0f);
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void timer(int v) {
    globalTime += 0.016f;
    updateAmphibians();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// ============================================================================
// INITIALIZATION & MAIN
// ============================================================================

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_pos[] = {10.0f, 20.0f, 10.0f, 0.0f};
    GLfloat light_amb[] = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat light_dif[] = {0.8f, 0.8f, 0.75f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
}

void setupFog() {
    // DISABLED: Fog was causing blur, keep everything CLEAR
    /*
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY, 0.02f);
    GLfloat fog_color[] = {0.5f, 0.7f, 1.0f, 1.0f};
    glFogfv(GL_FOG_COLOR, fog_color);
    */
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_NORMALIZE);
    
    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);  // Dark night background
    
    setupLighting();
    setupFog();
    
    initAmphibians();
    initTrees();
    initVolcanoes();
    initStars();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Amphibian Age - Volcanic Island Ecosystem");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}