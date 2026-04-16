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
    Vec3 pos;
    Vec3 basePos;      // Position relative to organism center
    float radius;
    int id;
};

struct TrailPoint {
    Vec3 pos;
    float alpha;
};

std::vector<Cell> cells;
std::vector<Vec3> fluidParticles;
std::vector<TrailPoint> trail;

Vec3 organismPosition(0.0f, 0.0f, -20.0f);
const int MAX_TRAIL_POINTS = 150;

// Forward declarations
void display();
void reshape(int w, int h);
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void setupLighting();
void init();

void generateFluidParticles() {
    fluidParticles.clear();
    for (int i = 0; i < 400; i++) {
        float x = (rand() % 100 - 50) / 10.0f;
        float y = (rand() % 100 - 50) / 10.0f;
        float z = (rand() % 100 - 50) / 10.0f;
        fluidParticles.push_back(Vec3(x, y, z));
    }
}

void drawFluidBackground() {
    // Draw ocean floor (sandy bottom)
    glDisable(GL_LIGHTING);
    glColor4f(0.6f, 0.5f, 0.3f, 0.4f);  // Sandy color
    glBegin(GL_QUADS);
    float floorZ = -30.0f;
    glVertex3f(-40.0f, -8.0f, floorZ);
    glVertex3f(40.0f, -8.0f, floorZ);
    glVertex3f(40.0f, -8.0f, floorZ - 5.0f);
    glVertex3f(-40.0f, -8.0f, floorZ - 5.0f);
    glEnd();
    glEnable(GL_LIGHTING);
    
    // Draw water surface waves (far up)
    glDisable(GL_LIGHTING);
    float surfaceY = 8.0f;
    glBegin(GL_TRIANGLE_STRIP);
    for (int x = -40; x <= 40; x += 5) {
        for (int z = -40; z <= 0; z += 5) {
            // Wave simulation
            float wave = sinf(globalTime + x * 0.1f) * 0.3f + cosf(globalTime * 0.7f + z * 0.08f) * 0.2f;
            float waveY = surfaceY + wave;
            
            // Fade ocean blue at surface, darker at depth
            float depthFade = 0.3f + (waveY - surfaceY) / 20.0f;
            glColor4f(0.1f + depthFade * 0.3f, 0.3f + depthFade * 0.4f, 0.5f + depthFade * 0.3f, 0.1f);
            
            glVertex3f(x, waveY, z);
            glVertex3f(x, waveY, z - 2.0f);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
    
    // Draw sunlight rays from above
    glDisable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 0.8f, 0.05f);
    glLineWidth(0.5f);
    glBegin(GL_LINES);
    for (int i = 0; i < 20; i++) {
        float rayX = -30.0f + (i / 20.0f) * 60.0f;
        float rayZ = -2.0f;
        
        float flicker = sinf(globalTime * 2.0f + i) * 0.1f;
        float rayWidth = 0.5f + flicker;
        
        glVertex3f(rayX - rayWidth, 10.0f, rayZ);
        glVertex3f(rayX - rayWidth * 0.5f, -8.0f, rayZ);
    }
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
    
    // Draw bubbles rising up (particles)
    glDisable(GL_LIGHTING);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (auto& p : fluidParticles) {
        // Bubbles rise upward
        p.y += 0.03f;  // Rise
        
        // Slight horizontal drift
        p.x += sinf(globalTime * 0.2f + p.z * 0.3f) * 0.01f;
        
        // Wrap around when reaching top
        if (p.y > 8.0f) {
            p.y = -8.0f;
            p.x = (rand() % 100 - 50) / 10.0f;
            p.z = (rand() % 100 - 50) / 10.0f;
        }
        
        // Keep in bounds
        if (p.x > 40.0f) p.x = -40.0f;
        if (p.x < -40.0f) p.x = 40.0f;
        if (p.z > 0.0f) p.z = -35.0f;
        if (p.z < -35.0f) p.z = 0.0f;
        
        // Bubble color - lighter/more visible toward surface
        float bubbleAlpha = 0.4f + (p.y / 16.0f) * 0.4f;
        glColor4f(0.7f, 0.85f, 1.0f, bubbleAlpha);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    glPointSize(1.0f);
    glEnable(GL_LIGHTING);
}

void drawCell(float x, float y, float z, float radius, int cellID) {
    float hue = fmodf(cellID * 0.618f, 1.0f);
    float r = sinf(hue * 3.14159f) * 0.5f + 0.3f;
    float g = sinf((hue + 0.33f) * 3.14159f) * 0.5f + 0.4f;
    float b = sinf((hue + 0.67f) * 3.14159f) * 0.5f + 0.6f;
    
    // Draw high-resolution cell
    int latSegments = 30;
    int lngSegments = 30;
    
    for (int i = 0; i < latSegments; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= lngSegments; j++) {
            for (int k = 0; k < 2; k++) {
                int lat = i + k;
                float lat0 = 3.14159f * lat / latSegments;
                float lng = 2.0f * 3.14159f * j / lngSegments;
                
                float distortion = sinf(lng * 3.0f + globalTime * 1.5f) * 0.08f;
                distortion += cosf(lat0 * 4.0f) * 0.06f;
                
                float rMod = radius * (1.0f + distortion);
                
                float px = x + rMod * sinf(lat0) * cosf(lng);
                float py = y + rMod * sinf(lat0) * sinf(lng);
                float pz = z + rMod * cosf(lat0);
                
                float colorVar = sinf(lat0 * 2.0f + lng) * 0.2f;
                glColor4f(r + colorVar, g + colorVar, b + colorVar, 0.7f);
                
                glVertex3f(px, py, pz);
            }
        }
        glEnd();
    }
}

void drawTrail() {
    // Draw faint trail lines showing movement path
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    
    for (size_t i = 0; i < trail.size(); i++) {
        float alpha = (i / (float)trail.size()) * 0.4f;
        glColor4f(0.8f, 0.6f, 1.0f, alpha);
        glVertex3f(trail[i].pos.x, trail[i].pos.y, trail[i].pos.z);
    }
    
    glEnd();
    glLineWidth(1.0f);
    
    // Draw trail points
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (size_t i = 0; i < trail.size(); i++) {
        float alpha = (i / (float)trail.size()) * 0.5f;
        glColor4f(0.9f, 0.7f, 1.0f, alpha);
        glVertex3f(trail[i].pos.x, trail[i].pos.y, trail[i].pos.z);
    }
    glEnd();
    glPointSize(1.0f);
}

void initializeCells() {
    if (!cells.empty()) return;
    
    int numCells = 40;
    float clusterRadius = 0.8f;
    
    for (int i = 0; i < numCells; i++) {
        float theta = (rand() % 1000 / 1000.0f) * 6.283f;
        float phi = (rand() % 1000 / 1000.0f) * 3.14159f;
        float r = (rand() % 1000 / 1000.0f) * clusterRadius;
        
        float x = r * sinf(phi) * cosf(theta);
        float y = r * sinf(phi) * sinf(theta);
        float z = r * cosf(phi);
        
        Vec3 basePos(x, y, z);
        cells.push_back({basePos, basePos, 0.25f, i});
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera follows organism slightly behind and above
    float cameraZ = organismPosition.z + 5.0f;
    float cameraY = 1.5f;
    
    gluLookAt(
        organismPosition.x, cameraY, cameraZ,
        organismPosition.x, 0.0f, organismPosition.z - 2.0f,
        0.0f, 1.0f, 0.0f
    );
    
    // Update organism position - SWIMMING MOTION
    float speed = 0.08f;  // Forward movement speed
    
    // Base forward movement
    organismPosition.z -= speed;
    
    // Wavy motion - swimming side to side
    float waveAmplitude = 0.5f;
    float waveFrequency = 0.5f;
    
    float waveX = sinf(globalTime * waveFrequency) * waveAmplitude;
    float waveY = cosf(globalTime * waveFrequency) * 0.3f;
    
    // Update cell positions with deformation
    for (auto& cell : cells) {
        float deformationX = sinf(globalTime * 1.5f + cell.id * 0.5f) * 0.1f;
        float deformationY = cosf(globalTime * 1.2f + cell.id * 0.3f) * 0.08f;
        float deformationZ = sinf(globalTime * 1.0f + cell.id * 0.4f) * 0.06f;
        
        // Pulsing scale
        float pulse = 1.0f + sinf(globalTime * 2.0f) * 0.08f;
        
        cell.pos.x = organismPosition.x + cell.basePos.x * pulse + deformationX + waveX;
        cell.pos.y = organismPosition.y + cell.basePos.y * pulse + deformationY + waveY;
        cell.pos.z = organismPosition.z + cell.basePos.z * pulse + deformationZ;
    }
    
    // Add to trail
    if ((int)globalTime % 1 == 0) {  // Add point every ~1 second
        trail.push_back({organismPosition, 0.8f});
        
        if (trail.size() > MAX_TRAIL_POINTS) {
            trail.erase(trail.begin());
        }
    }
    
    // Draw background fluid
    drawFluidBackground();
    
    // Draw trail
    drawTrail();
    
    // Apply organism rotation
    glPushMatrix();
    glTranslatef(organismPosition.x, organismPosition.y, organismPosition.z);
    glRotatef(globalTime * 5.0f, 0.3f, 1.0f, 0.2f);
    glTranslatef(-organismPosition.x, -organismPosition.y, -organismPosition.z);
    
    // Draw all cells
    for (const auto& cell : cells) {
        drawCell(cell.pos.x, cell.pos.y, cell.pos.z, cell.radius, cell.id);
    }
    
    glPopMatrix();
    
    // Draw text overlay
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    
    glColor4f(0.8f, 0.9f, 1.0f, 0.95f);
    glRasterPos2f(150, 50);
    const char* text = "Early Organism Movement - First Life Swims Through Ancient Oceans";
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
        const char* titleText = "Chapter 10: Cambrian Period - Explosion of Life";
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
    if (key == 27) {
        exit(0);
    }
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_position[] = {2.0f, 5.0f, 2.0f, 0.0f};
    GLfloat light_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat light_diffuse[] = {0.9f, 0.9f, 0.9f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

void init() {
    glClearColor(0.05f, 0.15f, 0.25f, 1.0f);  // Dark ocean blue background
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
    glutCreateWindow("Chapter 10: Early Organism Movement");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}
