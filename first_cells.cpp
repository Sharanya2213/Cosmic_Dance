#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>

float globalTime = 0.0f;
const float ANIMATION_DURATION = 25.0f;

struct Vec3 {
    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

struct Cell {
    Vec3 center;
    float radius;
    int id;
};

std::vector<Cell> cells;
std::vector<Vec3> fluidParticles;

int cellDivisionStage = 0; // 0=1cell, 1=2cells, 2=4cells, 3=8cells, 4=16cells, 5=32, 6=64, 7=128

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
        glColor4f(0.6f, 0.7f, 0.9f, 0.6f);  // Brighter dots
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
        
        // Animate particles
        p.x += sinf(globalTime + p.y * 0.5f) * 0.01f;
        p.y += cosf(globalTime + p.z * 0.5f) * 0.01f;
        
        // Boundary wrapping
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

void generateCellVertices(float centerX, float centerY, float centerZ, float radius, int numVertices = 80) {
    std::vector<Vec3> vertices;
    
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices / 2; j++) {
            float theta = (i / (float)numVertices) * 6.283f;
            float phi = (j / (float)(numVertices / 2)) * 3.14159f;
            
            // Irregular distortion
            float irregularity = sinf(theta * 3.0f + globalTime) * 0.1f;
            irregularity += sinf(phi * 4.0f) * 0.08f;
            
            float r = radius * (1.0f + irregularity);
            
            float x = centerX + r * sinf(phi) * cosf(theta);
            float y = centerY + r * sinf(phi) * sinf(theta);
            float z = centerZ + r * cosf(phi);
            
            vertices.push_back(Vec3(x, y, z));
        }
    }
}

void drawCell(float centerX, float centerY, float centerZ, float radius, int cellID) {
    // Generate unique color based on cell ID
    float hue = (cellID * 0.618f);  // Golden ratio for distributed colors
    float r = sinf(hue * 3.14159f) * 0.5f + 0.3f;
    float g = sinf((hue + 0.33f) * 3.14159f) * 0.5f + 0.4f;
    float b = sinf((hue + 0.67f) * 3.14159f) * 0.5f + 0.6f;
    
    // Draw high-resolution translucent cell membrane
    int latSegments = 50;   // Latitude segments
    int lngSegments = 50;   // Longitude segments
    
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
                distortion += cosf(globalTime * 1.5f + cellID) * 0.05f;  // Pulsing
                
                float rMod = radius * (1.0f + distortion);
                
                float x = centerX + rMod * sinf(lat0) * cosf(lng);
                float y = centerY + rMod * sinf(lat0) * sinf(lng);
                float z = centerZ + rMod * cosf(lat0);
                
                // Set color with high transparency
                float colorVariation = sinf(lat0 * 2.0f + lng) * 0.3f;
                glColor4f(
                    r + colorVariation, 
                    g + colorVariation, 
                    b + colorVariation, 
                    0.65f  // Translucent
                );
                
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
    
    // Draw cell outline for realism
    glColor4f(r * 1.2f, g * 1.2f, b * 1.2f, 0.8f);
    glPolygonMode(GL_FRONT, GL_LINE);
    glLineWidth(1.0f);
    
    for (int i = 0; i < 20; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= 20; j++) {
            for (int k = 0; k < 2; k++) {
                int lat = i + k;
                float lat0 = 3.14159f * lat / 20.0f;
                float lng = 2.0f * 3.14159f * j / 20.0f;
                
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
    float nucleusAlpha = 0.5f;
    glColor4f(1.0f, 0.3f, 1.0f, nucleusAlpha);
    
    for (int i = 0; i < 20; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= 20; j++) {
            for (int k = 0; k < 2; k++) {
                int lat = i + k;
                float lat0 = 3.14159f * lat / 20.0f;
                float lng = 2.0f * 3.14159f * j / 20.0f;
                
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
    
    // Draw mitochondria (organelles)
    glColor4f(1.0f, 0.7f, 0.2f, 0.6f);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for (int m = 0; m < 5; m++) {
        float angle1 = globalTime * 0.5f + (m / 5.0f) * 6.283f;
        float angle2 = globalTime * 0.3f + (m / 5.0f) * 6.283f;
        
        float mx = centerX + cosf(angle1) * (radius * 0.5f);
        float my = centerY + sinf(angle2) * (radius * 0.5f);
        float mz = centerZ + sinf(angle1 + angle2) * (radius * 0.4f);
        
        glVertex3f(mx, my, mz);
    }
    glEnd();
    glPointSize(1.0f);
}

void initializeCells() {
    cells.clear();
    
    // Phase timing for smooth transitions
    float phaseTime = fmodf(globalTime, ANIMATION_DURATION);
    float phaseDuration = ANIMATION_DURATION / 7.0f;
    float phaseProgress = fmodf(phaseTime, phaseDuration) / phaseDuration;  // 0 to 1 for current phase
    
    // Configuration for cell division stages - each stage emerges from parent cells
    if (cellDivisionStage == 0) {
        // 1 cell in center
        cells.push_back({Vec3(0.0f, 0.0f, 0.0f), 1.0f, 0});
    } 
    else if (cellDivisionStage == 1) {
        // 2 cells - emerging from 1, splitting along X axis
        float separation = phaseProgress * 1.0f;
        cells.push_back({Vec3(-0.5f - separation, 0.0f, 0.0f), 0.9f, 0});
        cells.push_back({Vec3(0.5f + separation, 0.0f, 0.0f), 0.9f, 1});
    } 
    else if (cellDivisionStage == 2) {
        // 4 cells - each parent cell (from stage 1) divides into 2, emerging in Y direction
        float separation = phaseProgress * 0.8f;
        // From first parent at (-0.5, 0, 0)
        cells.push_back({Vec3(-1.0f, -0.5f - separation, 0.0f), 0.75f, 0});
        cells.push_back({Vec3(-1.0f, 0.5f + separation, 0.0f), 0.75f, 1});
        // From second parent at (0.5, 0, 0)
        cells.push_back({Vec3(1.0f, -0.5f - separation, 0.0f), 0.75f, 2});
        cells.push_back({Vec3(1.0f, 0.5f + separation, 0.0f), 0.75f, 3});
    }
    else if (cellDivisionStage == 3) {
        // 8 cells - each of 4 cells divides into 2, emerging in Z direction from parent XY positions
        float separation = phaseProgress * 0.6f;
        // From cell at (-1.0, -0.5, 0)
        cells.push_back({Vec3(-1.0f, -0.5f, -0.5f - separation), 0.6f, 0});
        cells.push_back({Vec3(-1.0f, -0.5f, 0.5f + separation), 0.6f, 1});
        // From cell at (-1.0, 0.5, 0)
        cells.push_back({Vec3(-1.0f, 0.5f, -0.5f - separation), 0.6f, 2});
        cells.push_back({Vec3(-1.0f, 0.5f, 0.5f + separation), 0.6f, 3});
        // From cell at (1.0, -0.5, 0)
        cells.push_back({Vec3(1.0f, -0.5f, -0.5f - separation), 0.6f, 4});
        cells.push_back({Vec3(1.0f, -0.5f, 0.5f + separation), 0.6f, 5});
        // From cell at (1.0, 0.5, 0)
        cells.push_back({Vec3(1.0f, 0.5f, -0.5f - separation), 0.6f, 6});
        cells.push_back({Vec3(1.0f, 0.5f, 0.5f + separation), 0.6f, 7});
    }
    else if (cellDivisionStage == 4) {
        // 16 cells - each of 8 cells divides, emerging outward from their center
        float separation = phaseProgress * 0.4f;
        int id = 0;
        float parentPositions[8][3] = {
            {-1.0f, -0.5f, -0.5f}, {-1.0f, -0.5f, 0.5f},
            {-1.0f, 0.5f, -0.5f}, {-1.0f, 0.5f, 0.5f},
            {1.0f, -0.5f, -0.5f}, {1.0f, -0.5f, 0.5f},
            {1.0f, 0.5f, -0.5f}, {1.0f, 0.5f, 0.5f}
        };
        for (int p = 0; p < 8; p++) {
            // Split each parent into 2 cells along local X
            cells.push_back({Vec3(parentPositions[p][0] - 0.3f - separation, parentPositions[p][1], parentPositions[p][2]), 0.5f, id++});
            cells.push_back({Vec3(parentPositions[p][0] + 0.3f + separation, parentPositions[p][1], parentPositions[p][2]), 0.5f, id++});
        }
    }
    else if (cellDivisionStage == 5) {
        // 32 cells - expanding grid
        float separation = phaseProgress * 0.9f;
        int id = 0;
        for (float x = -1.4f; x <= 1.4f; x += 0.7f) {
            for (float y = -0.7f; y <= 0.7f; y += 0.7f) {
                for (float z = -0.6f; z <= 0.6f; z += 0.6f) {
                    float sx = x + (x > 0.1f ? separation : (x < -0.1f ? -separation : 0));
                    float sy = y + (y > 0.1f ? separation : (y < -0.1f ? -separation : 0));
                    cells.push_back({Vec3(sx, sy, z), 0.4f, id++});
                }
            }
        }
    }
    else if (cellDivisionStage == 6) {
        // 64 cells - finer grid
        float separation = phaseProgress * 2.0f;
        int id = 0;
        for (float x = -1.6f; x <= 1.6f; x += 0.53f) {
            for (float y = -0.8f; y <= 0.8f; y += 0.53f) {
                for (float z = -0.6f; z <= 0.6f; z += 0.6f) {
                    float sx = x + (x > 0.1f ? separation : (x < -0.1f ? -separation : 0));
                    float sy = y + (y > 0.1f ? separation : (y < -0.1f ? -separation : 0));
                    cells.push_back({Vec3(sx, sy, z), 0.35f, id++});
                }
            }
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Update cell division stage based on time
    float phaseTime = fmodf(globalTime, ANIMATION_DURATION);
    cellDivisionStage = (int)(phaseTime / (ANIMATION_DURATION / 7.0f));
    if (cellDivisionStage >= 7) cellDivisionStage = 6;
    
    initializeCells();
    
    // Draw fluid background
    drawFluidBackground();
    
    // Draw all cells
    for (const auto& cell : cells) {
        drawCell(cell.center.x, cell.center.y, cell.center.z, cell.radius, cell.id);
    }
    
    // Draw 2D text overlay
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(20.0f, 30.0f);
    
    const char* stageNames[] = {
        "1 Cell",
        "2 Cells (Division)",
        "4 Cells (Exponential Growth)",
        "8 Cells (Exponential Growth)",
        "16 Cells (Exponential Growth)",
        "32 Cells (Exponential Growth)",
        "64 Cells (Exponential Growth)"
    };
    
    for (const char* c = stageNames[cellDivisionStage]; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
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
        const char* titleText = "Chapter 8: Precambrian - The First Cells";
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
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 4.5f,    // Camera position
              0.0f, 0.0f, 0.0f,    // Look at
              0.0f, 1.0f, 0.0f);   // Up vector
}

void timer(int value) {
    if (globalTime < ANIMATION_DURATION) {
        globalTime += 0.016f;
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC
        exit(0);
    }
}

void init() {
    glClearColor(0.2f, 0.35f, 0.5f, 0.7f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    generateFluidParticles();
    
    printf("=== CELL DIVISION IN 3D ================\n");
    printf("Exponential cell multiplication\n");
    printf("1 → 2 → 4 → 8 → 16 cells\n");
    printf("Bluish-gray fluid environment\n");
    printf("Controls: ESC to exit | 25 sec loop - 1→2→4→8→16→32→64 cells (each emerging from parents)\n");
    printf("========================================\n");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutCreateWindow("Cell Division - 3D");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}
