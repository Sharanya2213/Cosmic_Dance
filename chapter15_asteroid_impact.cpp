#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>

float globalTime = 0.0f;

struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float life;
};

struct Asteroid {
    float x, y, z;
    float vx, vy, vz;
    float size;
    float rotation;
};

std::vector<Particle> debris;
std::vector<Particle> smoke;
std::vector<Asteroid> asteroids;
float impactTime = -1.0f; // Time when impact occurs
bool hasImpacted = false;

void initializeAsteroids() {
    asteroids.clear();
    // Large impact asteroid approaching from space
    asteroids.push_back({0.0f, 8.0f, -15.0f, 0.0f, -0.05f, 0.3f, 1.5f, 0.0f});
    
    // Smaller fragments in the belt
    for (int i = 0; i < 8; i++) {
        float angle = (i / 8.0f) * 6.283f;
        float speed = 0.15f + (rand() % 100 / 1000.0f);
        asteroids.push_back({
            5.0f * cosf(angle),
            5.0f + (rand() % 100 / 100.0f),
            -10.0f - (rand() % 50 / 10.0f),
            speed * cosf(angle),
            -0.02f - (rand() % 10 / 1000.0f),
            speed * sinf(angle),
            0.3f + (rand() % 20 / 100.0f),
            0.0f
        });
    }
}

void createExplosion(float x, float y, float z) {
    // Create debris particles
    for (int i = 0; i < 200; i++) {
        float angle = (rand() % 1000 / 1000.0f) * 6.283f;
        float elevation = (rand() % 1000 / 1000.0f) * 3.14159f;
        float speed = 0.1f + (rand() % 200 / 1000.0f);
        
        Particle p;
        p.x = x;
        p.y = y;
        p.z = z;
        p.vx = speed * sinf(elevation) * cosf(angle);
        p.vy = speed * sinf(elevation) * sinf(angle);
        p.vz = speed * cosf(elevation);
        p.life = 3.0f;
        
        debris.push_back(p);
    }
    
    // Create smoke particles
    for (int i = 0; i < 150; i++) {
        float angle = (rand() % 1000 / 1000.0f) * 6.283f;
        float radius = (rand() % 100 / 100.0f) * 0.5f;
        float speed = 0.02f + (rand() % 50 / 1000.0f);
        
        Particle p;
        p.x = x + radius * cosf(angle);
        p.y = y + (rand() % 100 / 100.0f);
        p.z = z + radius * sinf(angle);
        p.vx = speed * cosf(angle);
        p.vy = 0.01f + (rand() % 20 / 1000.0f);
        p.vz = speed * sinf(angle);
        p.life = 5.0f;
        
        smoke.push_back(p);
    }
}

void updateAsteroids() {
    for (auto& asteroid : asteroids) {
        asteroid.x += asteroid.vx;
        asteroid.y += asteroid.vy;
        asteroid.z += asteroid.vz;
        
        asteroid.rotation += 5.0f;
        
        // Check for impact
        if (!hasImpacted && asteroid.z > 0.0f && asteroid.vz > 0.0f) {
            if (asteroid.size > 1.0f) { // Only large asteroid causes impact
                impactTime = globalTime;
                hasImpacted = true;
                createExplosion(asteroid.x, asteroid.y, 0.1f);
            }
        }
    }
}

void updateParticles() {
    // Update debris
    for (auto it = debris.begin(); it != debris.end(); ) {
        it->x += it->vx;
        it->y += it->vy;
        it->z += it->vz;
        it->life -= 0.016f;
        it->vy -= 0.003f; // Gravity
        
        if (it->life <= 0.0f) {
            it = debris.erase(it);
        } else {
            ++it;
        }
    }
    
    // Update smoke
    for (auto it = smoke.begin(); it != smoke.end(); ) {
        it->x += it->vx;
        it->y += it->vy;
        it->z += it->vz;
        it->life -= 0.016f;
        
        if (it->life <= 0.0f) {
            it = smoke.erase(it);
        } else {
            ++it;
        }
    }
}

void drawAsteroid(float x, float y, float z, float size, float rotation) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0.3f, 0.7f, 0.4f);
    glColor4f(0.6f, 0.55f, 0.5f, 0.9f);
    
    // Rough crater-like surface
    int latSegments = 20;
    int lngSegments = 20;
    
    for (int i = 0; i < latSegments; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= lngSegments; j++) {
            for (int k = 0; k < 2; k++) {
                int lat = i + k;
                float lat0 = 3.14159f * lat / latSegments;
                float lng = 2.0f * 3.14159f * j / lngSegments;
                
                float distortion = sinf(lng * 4.0f + rotation * 0.01f) * 0.15f;
                distortion += cosf(lat0 * 3.0f) * 0.1f;
                
                float rMod = size * (1.0f + distortion);
                
                float px = rMod * sinf(lat0) * cosf(lng);
                float py = rMod * sinf(lat0) * sinf(lng);
                float pz = rMod * cosf(lat0);
                
                float colorVar = sinf(lat0 * 2.0f + lng) * 0.2f;
                glColor4f(
                    0.6f + colorVar,
                    0.55f + colorVar,
                    0.5f + colorVar,
                    0.9f
                );
                
                glVertex3f(px, py, pz);
            }
        }
        glEnd();
    }
    
    glPopMatrix();
}

void drawDebris() {
    glDisable(GL_LIGHTING);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    
    for (const auto& p : debris) {
        float brightness = p.life / 3.0f;
        glColor4f(1.0f, 0.7f + brightness * 0.2f, 0.3f, brightness * 0.8f);
        glVertex3f(p.x, p.y, p.z);
    }
    
    glEnd();
    glPointSize(1.0f);
    glEnable(GL_LIGHTING);
}

void drawSmoke() {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    
    for (const auto& p : smoke) {
        float alpha = (p.life / 5.0f) * 0.6f;
        glColor4f(0.3f, 0.3f, 0.3f, alpha);
        glVertex3f(p.x, p.y, p.z);
    }
    
    glEnd();
    glPointSize(1.0f);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void drawGround() {
    // Base ground
    glColor4f(0.3f, 0.5f, 0.2f, 0.95f);
    glBegin(GL_QUADS);
    glVertex3f(-15.0f, -0.05f, -5.0f);
    glVertex3f(15.0f, -0.05f, -5.0f);
    glVertex3f(15.0f, -0.05f, 5.0f);
    glVertex3f(-15.0f, -0.05f, 5.0f);
    glEnd();
    
    // Impact crater (if impacted)
    if (hasImpacted) {
        float craterGrow = (globalTime - impactTime) * 0.5f;
        float craterRadius = fminf(craterGrow, 3.0f);
        float craterDepth = sinf((globalTime - impactTime) * 2.0f) * 0.3f;
        
        glColor4f(0.2f, 0.2f, 0.2f, 0.9f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, -0.5f - craterDepth, 0.0f);
        
        for (int i = 0; i <= 32; i++) {
            float angle = (i / 32.0f) * 6.283f;
            float x = craterRadius * cosf(angle);
            float z = craterRadius * sinf(angle);
            float depth = craterDepth * (1.0f - (i / 32.0f));
            glVertex3f(x, -depth, z);
        }
        glEnd();
    }
}

void drawSky() {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    // Sky darkening effect when impacted
    float darkFactor = hasImpacted ? (1.0f - (globalTime - impactTime) * 0.1f) : 0.0f;
    
    glBegin(GL_QUADS);
    glColor4f(0.7f - darkFactor * 0.5f, 0.8f - darkFactor * 0.6f, 0.5f - darkFactor * 0.4f, 1.0f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    
    glColor4f(0.3f - darkFactor * 0.2f, 0.5f - darkFactor * 0.4f, 0.7f - darkFactor * 0.5f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
    
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera position
    float cameraZ = 15.0f;
    if (hasImpacted) {
        float shake = sinf(globalTime * 30.0f) * (3.0f - (globalTime - impactTime) * 0.5f);
        cameraZ += shake;
    }
    
    gluLookAt(
        0.0f, 2.0f, cameraZ,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );
    
    drawGround();
    
    // Draw asteroids
    for (auto& asteroid : asteroids) {
        drawAsteroid(asteroid.x, asteroid.y, asteroid.z, asteroid.size, asteroid.rotation);
    }
    
    // Draw explosion effects
    drawSmoke();
    drawDebris();
    
    // Text overlay
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    glColor4f(0.9f, 0.7f, 0.3f, 0.95f);
    glRasterPos2f(50, 50);
    const char* title = "Chapter 15: Asteroid Impact";
    for (const char* c = title; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    
    if (hasImpacted) {
        glColor4f(1.0f, 0.3f, 0.3f, 0.95f);
        glRasterPos2f(50, 80);
        const char* impact = "IMPACT! Extinction Event";
        for (const char* c = impact; *c; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    } else {
        glColor4f(0.8f, 0.9f, 1.0f, 0.95f);
        glRasterPos2f(50, 80);
        const char* approaching = "Asteroid approaching...";
        for (const char* c = approaching; *c; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
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
    
    updateAsteroids();
    updateParticles();
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_position[] = {5.0f, 8.0f, 5.0f, 0.0f};
    GLfloat light_ambient[] = {0.5f, 0.5f, 0.4f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 0.9f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    GLfloat material_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat material_shininess[] = {32.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
}

void init() {
    glClearColor(0.7f, 0.8f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    setupLighting();
    initializeAsteroids();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Chapter 15: Asteroid Impact - Extinction Event");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}
