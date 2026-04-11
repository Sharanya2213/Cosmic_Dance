#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>

float globalTime = 0.0f;
float creatureX = 0.0f;
float creatureY = 0.0f;
float creatureZ = 0.0f;

// Transformation parameters (0 to 1)
float evolutionProgress = 0.0f;

// Draw a simple sphere
void drawSphere(float radius, int slices, int stacks, float r, float g, float b) {
    glColor4f(r, g, b, 0.85f);
    glutSolidSphere(radius, slices, stacks);
}

// Draw a cylinder for limbs
void drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
    GLUquadric* quad = gluNewQuadric();
    glColor4f(0.5f, 0.65f, 0.55f, 0.85f);
    gluCylinder(quad, baseRadius, topRadius, height, slices, stacks);
}

// Draw the environment
void drawEnvironment(float landProgress) {
    glDisable(GL_LIGHTING);
    
    // Water (blue background)
    float waterAlpha = 1.0f - landProgress;
    glColor4f(0.1f, 0.2f, 0.4f, waterAlpha * 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-15.0f, 1.5f, -20.0f);
    glVertex3f(15.0f, 1.5f, -20.0f);
    glVertex3f(15.0f, 1.5f, 0.0f);
    glVertex3f(-15.0f, 1.5f, 0.0f);
    glEnd();
    
    // Ground (brown/green)
    float groundAlpha = landProgress;
    float groundGreen = 0.2f + landProgress * 0.3f;
    float groundBrown = 0.15f + landProgress * 0.2f;
    glColor4f(groundBrown, groundGreen, 0.1f, groundAlpha * 0.95f);
    glBegin(GL_QUADS);
    glVertex3f(-15.0f, -1.5f, -20.0f);
    glVertex3f(15.0f, -1.5f, -20.0f);
    glVertex3f(15.0f, -1.5f, 0.0f);
    glVertex3f(-15.0f, -1.5f, 0.0f);
    glEnd();
    
    // Transition plane
    glColor4f(0.3f, 0.25f, 0.2f, 0.7f);
    glBegin(GL_LINES);
    for (int i = -15; i <= 15; i += 2) {
        glVertex3f(i, -0.1f, -20.0f);
        glVertex3f(i, -0.1f, 0.0f);
        glVertex3f(-15.0f, -0.1f, i + 20.0f);
        glVertex3f(15.0f, -0.1f, i + 20.0f);
    }
    glEnd();
    
    glEnable(GL_LIGHTING);
}

// Draw the creature
void drawCreature(float x, float y, float z, float progress) {
    // progress: 0 = fish, 1 = land creature
    
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // HEAD (stays mostly the same)
    glPushMatrix();
    glTranslatef(0.8f, 0.0f, 0.0f);
    glColor4f(0.5f, 0.7f, 0.6f, 0.9f);
    glutSolidSphere(0.35f, 16, 16);
    
    // Eye
    glPushMatrix();
    glTranslatef(0.2f, 0.15f, 0.2f);
    glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
    glutSolidSphere(0.08f, 8, 8);
    glPopMatrix();
    glPopMatrix();
    
    // BODY (becomes thicker)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    float bodyScaleY = 1.0f + progress * 0.4f;  // Gets taller
    float bodyScaleX = 2.5f - progress * 0.8f;  // Gets shorter
    glScalef(bodyScaleX, bodyScaleY, 1.0f);
    glColor4f(0.4f, 0.7f, 0.6f, 0.85f);
    glutSolidSphere(0.5f, 20, 20);
    glPopMatrix();
    
    // TAIL (shrinks away)
    float tailScale = 1.0f - progress * 0.9f;  // Tail mostly disappears
    if (tailScale > 0.05f) {
        glPushMatrix();
        glTranslatef(-0.7f, 0.0f, 0.0f);
        float tailWave = sinf(globalTime * 3.0f) * (1.0f - progress) * 0.2f;
        glRotatef(tailWave * 20.0f, 0.0f, 1.0f, 0.0f);
        glScalef(tailScale, tailScale, tailScale);
        glColor4f(0.35f, 0.65f, 0.55f, 0.8f);
        glutSolidCone(0.25f, 0.8f, 12, 12);
        glPopMatrix();
    }
    
    // FRONT LEFT LEG (emerges)
    if (progress > 0.1f) {
        glPushMatrix();
        glTranslatef(0.3f, -0.6f, 0.35f);
        float legProgress = (progress - 0.1f) / 0.9f;
        legProgress = fmaxf(0.0f, fminf(1.0f, legProgress));
        
        float legSwing = sinf(globalTime * 2.0f) * legProgress * 0.3f;
        glRotatef(legSwing * 30.0f + legProgress * 30.0f, 1.0f, 0.0f, 0.0f);
        
        glScalef(legProgress, legProgress, legProgress);
        drawCylinder(0.15f, 0.12f, 0.6f, 12, 12);
        
        // Foot
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.6f);
        glColor4f(0.5f, 0.65f, 0.55f, 0.85f);
        glutSolidSphere(0.12f, 8, 8);
        glPopMatrix();
        glPopMatrix();
    }
    
    // FRONT RIGHT LEG
    if (progress > 0.1f) {
        glPushMatrix();
        glTranslatef(0.3f, -0.6f, -0.35f);
        float legProgress = (progress - 0.1f) / 0.9f;
        legProgress = fmaxf(0.0f, fminf(1.0f, legProgress));
        
        float legSwing = sinf(globalTime * 2.0f + 3.14159f) * legProgress * 0.3f;
        glRotatef(legSwing * 30.0f + legProgress * 30.0f, 1.0f, 0.0f, 0.0f);
        
        glScalef(legProgress, legProgress, legProgress);
        drawCylinder(0.15f, 0.12f, 0.6f, 12, 12);
        
        // Foot
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.6f);
        glColor4f(0.5f, 0.65f, 0.55f, 0.85f);
        glutSolidSphere(0.12f, 8, 8);
        glPopMatrix();
        glPopMatrix();
    }
    
    // REAR LEFT LEG
    if (progress > 0.2f) {
        glPushMatrix();
        glTranslatef(-0.3f, -0.6f, 0.35f);
        float legProgress = (progress - 0.2f) / 0.8f;
        legProgress = fmaxf(0.0f, fminf(1.0f, legProgress));
        
        float legSwing = sinf(globalTime * 2.0f + 3.14159f) * legProgress * 0.3f;
        glRotatef(legSwing * 30.0f + legProgress * 30.0f, 1.0f, 0.0f, 0.0f);
        
        glScalef(legProgress, legProgress, legProgress);
        drawCylinder(0.15f, 0.12f, 0.6f, 12, 12);
        
        // Foot
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.6f);
        glColor4f(0.5f, 0.65f, 0.55f, 0.85f);
        glutSolidSphere(0.12f, 8, 8);
        glPopMatrix();
        glPopMatrix();
    }
    
    // REAR RIGHT LEG
    if (progress > 0.2f) {
        glPushMatrix();
        glTranslatef(-0.3f, -0.6f, -0.35f);
        float legProgress = (progress - 0.2f) / 0.8f;
        legProgress = fmaxf(0.0f, fminf(1.0f, legProgress));
        
        float legSwing = sinf(globalTime * 2.0f) * legProgress * 0.3f;
        glRotatef(legSwing * 30.0f + legProgress * 30.0f, 1.0f, 0.0f, 0.0f);
        
        glScalef(legProgress, legProgress, legProgress);
        drawCylinder(0.15f, 0.12f, 0.6f, 12, 12);
        
        // Foot
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.6f);
        glColor4f(0.5f, 0.65f, 0.55f, 0.85f);
        glutSolidSphere(0.12f, 8, 8);
        glPopMatrix();
        glPopMatrix();
    }
    
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera follows creature from side
    float cameraX = creatureX + 2.5f;
    float cameraY = 0.5f + evolutionProgress * 0.5f;
    float cameraZ = 8.0f;
    
    gluLookAt(
        cameraX, cameraY, cameraZ,
        creatureX, 0.0f, creatureZ - 2.0f,
        0.0f, 1.0f, 0.0f
    );
    
    // Update creature position
    if (evolutionProgress < 1.0f) {
        // Swimming motion (early stage)
        float swimSpeed = 0.01f * (1.0f - evolutionProgress * 0.5f);
        creatureX += swimSpeed;
        creatureY = sinf(globalTime * 1.5f + creatureX * 2.0f) * 0.2f * (1.0f - evolutionProgress);
    } else {
        // Walking motion (later stage)
        float walkSpeed = 0.008f;
        creatureX += walkSpeed;
        creatureY = 0.0f;
    }
    
    // Draw environment
    drawEnvironment(evolutionProgress);
    
    // Draw creature
    drawCreature(creatureX, creatureY, creatureZ, evolutionProgress);
    
    // Draw text overlay
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glColor4f(0.7f, 0.9f, 1.0f, 0.95f);
    glRasterPos2f(50, 50);
    const char* title = "Chapter 3: Transition from Water to Land";
    for (const char* c = title; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    
    // Show evolution progress
    glRasterPos2f(50, 80);
    char progress_text[64];
    sprintf(progress_text, "Evolution: %.0f%%", evolutionProgress * 100.0f);
    for (const char* c = progress_text; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
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
    
    // Evolution progresses over 60 seconds
    evolutionProgress = fminf(globalTime / 60.0f, 1.0f);
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_position[] = { 3.0f, 6.0f, 3.0f, 0.0f };
    GLfloat light_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat light_diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void init() {
    glClearColor(0.15f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_SMOOTH);
    
    setupLighting();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Chapter 3: Transition from Water to Land");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}
