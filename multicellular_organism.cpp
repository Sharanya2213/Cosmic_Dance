#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdio>

float globalTime = 0.0f;

struct Vec3 {
    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    float distance(const Vec3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return sqrtf(dx*dx + dy*dy + dz*dz);
    }
    
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    
    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
};

struct Cell {
    Vec3 pos;           // Current position
    Vec3 targetPos;     // Target cluster position
    Vec3 initialPos;    // Starting scattered position
    float radius;
    int id;
};

std::vector<Cell> cells;
std::vector<Vec3> fluidParticles;

// Forward declarations
void display();
void reshape(int w, int h);
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void setupLighting();
void init();

void generateFluidParticles() {
    fluidParticles.clear();
    for (int i = 0; i < 300; i++) {
        float x = (rand() % 100 - 50) / 10.0f;
        float y = (rand() % 100 - 50) / 10.0f;
        float z = (rand() % 100 - 50) / 10.0f;
        fluidParticles.push_back(Vec3(x, y, z));
    }
}

void drawFluidBackground() {
    // Draw static star-like dots in background
    glPointSize(2.5f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 200; i++) {
        float x = cosf(i * 0.314f) * 8.0f;
        float y = sinf(i * 0.271f) * 8.0f;
        float z = cosf(i * 0.159f) * 8.0f - 7.0f;
        glColor4f(0.6f, 0.7f, 0.9f, 0.6f);
        glVertex3f(x, y, z);
    }
    glEnd();
    glPointSize(1.0f);
    
    // Draw animated fluid particles
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for (auto& p : fluidParticles) {
        float alpha = 0.15f;
        glColor4f(0.3f, 0.5f, 0.7f, alpha);
        
        p.x += sinf(globalTime + p.y * 0.5f) * 0.01f;
        p.y += cosf(globalTime + p.z * 0.5f) * 0.01f;
        
        if (p.x > 5.0f) p.x = -5.0f;
        if (p.x < -5.0f) p.x = 5.0f;
        if (p.y > 5.0f) p.y = -5.0f;
        if (p.y < -5.0f) p.y = 5.0f;
        if (p.z > 5.0f) p.z = -5.0f;
        if (p.z < -5.0f) p.z = 5.0f;
        
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    glPointSize(1.0f);
}


void drawCell(float centerX, float centerY, float centerZ, float radius, int cellID) {
    // Generate unique color based on cell ID using golden ratio
    float hue = fmodf(cellID * 0.618f, 1.0f);
    float r = sinf(hue * 3.14159f) * 0.5f + 0.3f;
    float g = sinf((hue + 0.33f) * 3.14159f) * 0.5f + 0.4f;
    float b = sinf((hue + 0.67f) * 3.14159f) * 0.5f + 0.6f;
    
    // Draw high-resolution translucent cell membrane
    int latSegments = 40;
    int lngSegments = 40;
    
    for (int i = 0; i < latSegments; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= lngSegments; j++) {
            for (int k = 0; k < 2; k++) {
                int lat = i + k;
                float lat0 = 3.14159f * lat / latSegments;
                float lng = 2.0f * 3.14159f * j / lngSegments;
                
                // Unique distortion pattern per cell
                float distortion = sinf(lng * (3.0f + cellID * 0.5f) + globalTime) * 0.12f;
                distortion += sinf(lat0 * (4.0f + cellID * 0.3f)) * 0.08f;
                distortion += cosf(globalTime * 1.5f + cellID) * 0.05f;
                
                float rMod = radius * (1.0f + distortion);
                
                float x = centerX + rMod * sinf(lat0) * cosf(lng);
                float y = centerY + rMod * sinf(lat0) * sinf(lng);
                float z = centerZ + rMod * cosf(lat0);
                
                float colorVariation = sinf(lat0 * 2.0f + lng) * 0.3f;
                glColor4f(
                    r + colorVariation,
                    g + colorVariation,
                    b + colorVariation,
                    0.65f
                );
                
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
    
    // Draw cell outline
    glColor4f(r * 1.2f, g * 1.2f, b * 1.2f, 0.8f);
    glPolygonMode(GL_FRONT, GL_LINE);
    glLineWidth(0.8f);
    
    for (int i = 0; i < 15; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= 15; j++) {
            for (int k = 0; k < 2; k++) {
                int lat = i + k;
                float lat0 = 3.14159f * lat / 15.0f;
                float lng = 2.0f * 3.14159f * j / 15.0f;
                
                float distortion = sinf(lng * (3.0f + cellID * 0.5f) + globalTime) * 0.12f;
                distortion += sinf(lat0 * (4.0f + cellID * 0.3f)) * 0.08f;
                
                float rMod = radius * (1.0f + distortion);
                
                float x = centerX + rMod * sinf(lat0) * cosf(lng);
                float y = centerY + rMod * sinf(lat0) * sinf(lng);
                float z = centerZ + rMod * cosf(lat0);
                
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
    
    glPolygonMode(GL_FRONT, GL_FILL);
    
    // Draw nucleus (internal structure)
    float nucleusRadius = radius * 0.35f;
    glColor4f(1.0f, 0.3f, 1.0f, 0.5f);
    
    for (int i = 0; i < 15; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= 15; j++) {
            for (int k = 0; k < 2; k++) {
                int lat = i + k;
                float lat0 = 3.14159f * lat / 15.0f;
                float lng = 2.0f * 3.14159f * j / 15.0f;
                
                float pulse = sinf(globalTime * 2.0f) * 0.1f;
                float nRad = nucleusRadius * (1.0f + pulse);
                
                float x = centerX + nRad * sinf(lat0) * cosf(lng);
                float y = centerY + nRad * sinf(lat0) * sinf(lng);
                float z = centerZ + nRad * cosf(lat0);
                
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
    
    // Draw mitochondria
    glColor4f(1.0f, 0.7f, 0.2f, 0.6f);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int m = 0; m < 4; m++) {
        float angle1 = globalTime * 0.5f + (m / 4.0f) * 6.283f;
        float angle2 = globalTime * 0.3f + (m / 4.0f) * 6.283f;
        
        float mx = centerX + cosf(angle1) * (radius * 0.5f);
        float my = centerY + sinf(angle2) * (radius * 0.5f);
        float mz = centerZ + sinf(angle1 + angle2) * (radius * 0.4f);
        
        glVertex3f(mx, my, mz);
    }
    glEnd();
    glPointSize(1.0f);
}


void initializeCells() {
    if (!cells.empty()) return; // Only initialize once
    
    int lumps = 5;
    int cellsPerLump = 10;
    
    for (int l = 0; l < lumps; l++) {
        // Target cluster position for each lump
        float lumpAngle = (l / (float)lumps) * 6.283f;
        float lumpX = cosf(lumpAngle) * 1.2f;
        float lumpY = sinf(lumpAngle) * 0.8f;
        float lumpZ = cosf(lumpAngle * 0.7f) * 0.6f;
        
        // Add cells to each lump
        for (int i = 0; i < cellsPerLump; i++) {
            float angle = (i / (float)cellsPerLump) * 6.283f;
            float phi = (i / (float)cellsPerLump) * 3.14159f;
            
            // Target position (packed tightly within lump)
            float targetX = lumpX + cosf(angle) * 0.35f + sinf(phi) * 0.15f;
            float targetY = lumpY + sinf(angle) * 0.35f + cosf(phi) * 0.12f;
            float targetZ = lumpZ + sinf(angle * 0.7f) * 0.25f + cosf(phi) * 0.1f;
            
            // Initial position - randomly scattered in space
            float initAngle1 = (rand() % 1000 / 1000.0f) * 6.283f;
            float initAngle2 = (rand() % 1000 / 1000.0f) * 6.283f;
            float initRadius = 2.5f + (rand() % 1000 / 1000.0f) * 1.5f;
            
            float initX = cosf(initAngle1) * initRadius;
            float initY = sinf(initAngle2) * initRadius;
            float initZ = cosf(initAngle1 + initAngle2) * initRadius;
            
            int cellID = l * cellsPerLump + i;
            Vec3 initialPos(initX, initY, initZ);
            Vec3 targetPos(targetX, targetY, targetZ);
            
            cells.push_back({
                initialPos,              // pos (starts at initial position)
                targetPos,               // targetPos
                initialPos,              // initialPos
                0.35f,                   // radius
                cellID                   // id
            });
        }
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera positioned to see 3D structure
    gluLookAt(0, 1.5f, 4.0f,    // Camera position
              0, 0, 0,            // Look at
              0, 1, 0);            // Up vector
    
    // Update cell positions based on animation progress
    // Formation takes 10 seconds, then holds for 15 seconds, then repeats
    float formationTime = 10.0f;
    float totalLoopTime = formationTime + 15.0f;
    float loopTime = fmodf(globalTime, totalLoopTime);
    float formationProgress = fminf(loopTime / formationTime, 1.0f);  // 0 to 1 during formation, then stays at 1
    
    // Apply easing function for smooth formation
    float eased = formationProgress * formationProgress * (3.0f - 2.0f * formationProgress); // Smoothstep
    
    for (auto& cell : cells) {
        // Interpolate from initial to target position
        cell.pos.x = cell.initialPos.x + (cell.targetPos.x - cell.initialPos.x) * eased;
        cell.pos.y = cell.initialPos.y + (cell.targetPos.y - cell.initialPos.y) * eased;
        cell.pos.z = cell.initialPos.z + (cell.targetPos.z - cell.initialPos.z) * eased;
    }
    
    // Draw background
    drawFluidBackground();
    
    // Apply global rotation and floating motion
    glPushMatrix();
    glRotatef(globalTime * 15.0f, 0.3f, 1.0f, 0.2f);
    glTranslatef(0, sinf(globalTime * 0.5f) * 0.3f, 0);
    
    // Draw connection network between nearby cells
    glColor4f(0.6f, 0.7f, 0.9f, 0.2f);
    glLineWidth(0.5f);
    glBegin(GL_LINES);
    for (size_t i = 0; i < cells.size(); i++) {
        for (size_t j = i + 1; j < cells.size(); j++) {
            float dist = cells[i].pos.distance(cells[j].pos);
            if (dist < 1.0f) {
                glVertex3f(cells[i].pos.x, cells[i].pos.y, cells[i].pos.z);
                glVertex3f(cells[j].pos.x, cells[j].pos.y, cells[j].pos.z);
            }
        }
    }
    glEnd();
    glLineWidth(1.0f);
    
    // Draw all cells
    for (const auto& cell : cells) {
        drawCell(cell.pos.x, cell.pos.y, cell.pos.z, cell.radius, cell.id);
    }
    
    glPopMatrix();
    
    // Draw title/status
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 600, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    
    glColor4f(0.8f, 0.8f, 1.0f, 0.9f);
    glRasterPos2f(220, 50);
    
    const char* text = "Cells Forming Multicellular Cluster";
    for (const char* c = text; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    
    glEnable(GL_DEPTH_TEST);
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
        const char* titleText = "Chapter 9: Precambrian - Multicellular Life";
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

void timer(int value) {
    globalTime += 0.016f;
    if (globalTime >= 18.0f) exit(0);
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC
        exit(0);
    }
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_position[] = {2.0f, 3.0f, 2.0f, 0.0f};
    GLfloat light_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}


void init() {
    glClearColor(0.01f, 0.01f, 0.02f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    setupLighting();
    generateFluidParticles();
    initializeCells();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Chapter 9: Multicellular Organism Formation");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}
