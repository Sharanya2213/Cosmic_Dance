#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdio>

float globalTime = 0.0f;

struct Animal {
    float x, z;
    float scale;
    float speed;
    int type;  // 0=Leopard, 1=Hyena, 2=Rhino, 3=Giraffe, 4=Antelope
};

std::vector<Animal> animals;

void initScene() {
    animals.clear();
    
    // Leopards (6 total) - Closer positions
    animals.push_back({-12.0f, 8.0f, 0.8f, 0.003f, 0});
    animals.push_back({-5.0f, -8.0f, 0.7f, 0.0035f, 0});
    animals.push_back({3.0f, 6.0f, 0.75f, 0.0032f, 0});
    animals.push_back({10.0f, -5.0f, 0.82f, 0.0034f, 0});
    animals.push_back({-18.0f, 2.0f, 0.78f, 0.0036f, 0});
    animals.push_back({16.0f, 9.0f, 0.72f, 0.0038f, 0});
    
    // Hyenas (6 total) - Closer positions
    animals.push_back({-15.0f, -3.0f, 0.75f, 0.0032f, 1});
    animals.push_back({6.0f, 7.0f, 0.72f, 0.0035f, 1});
    animals.push_back({2.0f, -10.0f, 0.78f, 0.0033f, 1});
    animals.push_back({-8.0f, 10.0f, 0.7f, 0.0031f, 1});
    animals.push_back({12.0f, 4.0f, 0.76f, 0.0037f, 1});
    animals.push_back({-20.0f, -7.0f, 0.74f, 0.0034f, 1});
    
    // Rhinos (4 total) - Closer positions
    animals.push_back({-3.0f, 1.0f, 1.1f, 0.0025f, 2});
    animals.push_back({8.0f, -2.0f, 1.0f, 0.0024f, 2});
    animals.push_back({-10.0f, 11.0f, 1.15f, 0.0026f, 2});
    animals.push_back({14.0f, 3.0f, 1.05f, 0.0027f, 2});
    
    // Giraffes (6 total) - Closer positions
    animals.push_back({-16.0f, 12.0f, 1.3f, 0.002f, 3});
    animals.push_back({5.0f, 14.0f, 1.2f, 0.0022f, 3});
    animals.push_back({-22.0f, -15.0f, 1.4f, 0.0018f, 3});
    animals.push_back({18.0f, 16.0f, 1.15f, 0.0021f, 3});
    animals.push_back({-1.0f, 13.0f, 1.25f, 0.0020f, 3});
    animals.push_back({-24.0f, 6.0f, 1.28f, 0.0019f, 3});
    
    // Antelopes (8 total) - Closer positions
    animals.push_back({1.0f, 0.0f, 0.6f, 0.0045f, 4});
    animals.push_back({-11.0f, -10.0f, 0.65f, 0.0048f, 4});
    animals.push_back({9.0f, 5.0f, 0.62f, 0.0050f, 4});
    animals.push_back({-19.0f, 5.0f, 0.58f, 0.0044f, 4});
    animals.push_back({15.0f, -8.0f, 0.68f, 0.0046f, 4});
    animals.push_back({-4.0f, -14.0f, 0.61f, 0.0052f, 4});
    animals.push_back({20.0f, 8.0f, 0.64f, 0.0047f, 4});
    animals.push_back({-25.0f, -12.0f, 0.59f, 0.0043f, 4});
    
    printf("Initialized 30-animal savanna ecosystem (clustered view)\n");
}

void drawSky() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Light blue sky gradient
    glBegin(GL_QUADS);
    // Light blue top
    glColor3f(0.5f, 0.8f, 1.0f);
    glVertex3f(-150.0f, 40.0f, -150.0f);
    glVertex3f(150.0f, 40.0f, -150.0f);
    
    // Lighter blue/white bottom
    glColor3f(0.7f, 0.9f, 1.0f);
    glVertex3f(150.0f, 5.0f, -150.0f);
    glVertex3f(-150.0f, 5.0f, -150.0f);
    glEnd();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void drawGround() {
    // Golden savanna grass
    glColor3f(0.85f, 0.75f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-150.0f, 0.0f, -150.0f);
    glVertex3f(150.0f, 0.0f, -150.0f);
    glVertex3f(150.0f, 0.0f, 150.0f);
    glVertex3f(-150.0f, 0.0f, 150.0f);
    glEnd();
    
    // Grass texture detail
    glColor3f(0.75f, 0.65f, 0.2f);
    glBegin(GL_LINES);
    glLineWidth(0.5f);
    for (float x = -150.0f; x < 150.0f; x += 8.0f) {
        for (float z = -150.0f; z < 150.0f; z += 8.0f) {
            glVertex3f(x, 0.01f, z);
            glVertex3f(x + 4.0f, 0.01f, z + 4.0f);
        }
    }
    glLineWidth(1.0f);
    glEnd();
}

void drawTree(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Trunk
    glColor3f(0.55f, 0.4f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(0.4f, 1.0f, 0.4f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.25f, 3.0f, 8, 8);
    glPopMatrix();
    
    // Canopy (acacia tree style)
    glColor3f(0.35f, 0.55f, 0.25f);
    glPushMatrix();
    glTranslatef(0.0f, 2.8f, 0.0f);
    glScalef(1.2f, 0.8f, 1.0f);
    glutSolidSphere(0.8f, 12, 12);
    glPopMatrix();
    
    glPopMatrix();
}

void drawLeopard(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Chest (front body)
    glColor3f(0.82f, 0.72f, 0.42f);
    glPushMatrix();
    glTranslatef(0.3f, 0.2f, 0.0f);
    glScalef(1.2f, 1.1f, 0.75f);
    glutSolidSphere(0.5f, 14, 14);
    glPopMatrix();
    
    // Abdomen (main body)
    glColor3f(0.8f, 0.7f, 0.4f);
    glPushMatrix();
    glTranslatef(-0.4f, 0.0f, 0.0f);
    glScalef(1.5f, 0.95f, 0.75f);
    glutSolidSphere(0.55f, 14, 14);
    glPopMatrix();
    
    // Hindquarters
    glColor3f(0.78f, 0.68f, 0.38f);
    glPushMatrix();
    glTranslatef(-1.1f, -0.15f, 0.0f);
    glScalef(0.9f, 0.9f, 0.7f);
    glutSolidSphere(0.48f, 12, 12);
    glPopMatrix();
    
    // Head
    glColor3f(0.84f, 0.74f, 0.47f);
    glPushMatrix();
    glTranslatef(1.3f, 0.5f, 0.0f);
    glScalef(1.0f, 0.9f, 0.85f);
    glutSolidSphere(0.48f, 12, 12);
    
    // Snout
    glColor3f(0.9f, 0.8f, 0.5f);
    glPushMatrix();
    glTranslatef(0.35f, -0.15f, 0.0f);
    glScalef(0.65f, 0.5f, 0.6f);
    glutSolidSphere(0.28f, 10, 10);
    glPopMatrix();
    
    // Eyes
    glColor3f(0.15f, 0.15f, 0.1f);
    glPushMatrix();
    glTranslatef(0.2f, 0.15f, 0.25f);
    glutSolidSphere(0.08f, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.2f, 0.15f, -0.25f);
    glutSolidSphere(0.08f, 8, 8);
    glPopMatrix();
    glPopMatrix();
    
    // Ears (tall, alert)
    glColor3f(0.72f, 0.62f, 0.32f);
    glPushMatrix();
    glTranslatef(1.5f, 1.0f, 0.2f);
    glScalef(0.28f, 0.55f, 0.25f);
    glutSolidSphere(0.22f, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.5f, 1.0f, -0.2f);
    glScalef(0.28f, 0.55f, 0.25f);
    glutSolidSphere(0.22f, 8, 8);
    glPopMatrix();
    
    // Front left leg (muscular)
    glColor3f(0.76f, 0.66f, 0.36f);
    glPushMatrix();
    glTranslatef(0.5f, -0.8f, 0.35f);
    glScalef(0.32f, 1.6f, 0.32f);
    glutSolidSphere(0.3f, 10, 10);
    glPopMatrix();
    
    // Front right leg
    glPushMatrix();
    glTranslatef(0.5f, -0.8f, -0.35f);
    glScalef(0.32f, 1.6f, 0.32f);
    glutSolidSphere(0.3f, 10, 10);
    glPopMatrix();
    
    // Back left leg
    glPushMatrix();
    glTranslatef(-0.9f, -0.8f, 0.38f);
    glScalef(0.33f, 1.55f, 0.33f);
    glutSolidSphere(0.3f, 10, 10);
    glPopMatrix();
    
    // Back right leg
    glPushMatrix();
    glTranslatef(-0.9f, -0.8f, -0.38f);
    glScalef(0.33f, 1.55f, 0.33f);
    glutSolidSphere(0.3f, 10, 10);
    glPopMatrix();
    
    // Rosette spots on body
    glColor3f(0.5f, 0.4f, 0.1f);
    for (int i = 0; i < 8; i++) {
        float spotX = -0.5f + (i % 4) * 0.5f;
        float spotZ = (i / 4) * 0.4f - 0.2f;
        glPushMatrix();
        glTranslatef(spotX, 0.3f, spotZ);
        glutSolidSphere(0.15f, 6, 6);
        glPopMatrix();
    }
    
    // Tail (long, curved)
    glColor3f(0.76f, 0.66f, 0.36f);
    glPushMatrix();
    glTranslatef(-1.6f, 0.1f, 0.0f);
    float tailSway = sinf(globalTime * 4.0f) * 15.0f;
    glRotatef(tailSway, 0.0f, 0.0f, 1.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.18f, 2.0f, 8, 8);
    glPopMatrix();
    
    glPopMatrix();
}

void drawHyena(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Front shoulders (massive)
    glColor3f(0.62f, 0.52f, 0.42f);
    glPushMatrix();
    glTranslatef(0.2f, 0.15f, 0.0f);
    glScalef(1.3f, 1.25f, 0.8f);
    glutSolidSphere(0.58f, 12, 12);
    glPopMatrix();
    
    // Back body (sloped down)
    glColor3f(0.65f, 0.55f, 0.45f);
    glPushMatrix();
    glTranslatef(-0.5f, -0.3f, 0.0f);
    glScalef(1.4f, 0.9f, 0.85f);
    glutSolidSphere(0.52f, 12, 12);
    glPopMatrix();
    
    // Head (powerful jaws)
    glColor3f(0.68f, 0.58f, 0.48f);
    glPushMatrix();
    glTranslatef(1.1f, 0.6f, 0.0f);
    glScalef(0.95f, 0.85f, 0.8f);
    glutSolidSphere(0.45f, 12, 12);
    
    // Muzzle
    glColor3f(0.72f, 0.62f, 0.52f);
    glPushMatrix();
    glTranslatef(0.4f, -0.1f, 0.0f);
    glScalef(0.7f, 0.55f, 0.65f);
    glutSolidSphere(0.3f, 10, 10);
    glPopMatrix();
    
    // Eyes (forward-facing, aggressive)
    glColor3f(0.2f, 0.15f, 0.1f);
    glPushMatrix();
    glTranslatef(0.15f, 0.1f, 0.22f);
    glutSolidSphere(0.07f, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.15f, 0.1f, -0.22f);
    glutSolidSphere(0.07f, 8, 8);
    glPopMatrix();
    glPopMatrix();
    
    // Ears (short, rounded, back-angled)
    glColor3f(0.58f, 0.48f, 0.38f);
    glPushMatrix();
    glTranslatef(1.25f, 0.95f, 0.25f);
    glScalef(0.25f, 0.4f, 0.25f);
    glutSolidSphere(0.2f, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.25f, 0.95f, -0.25f);
    glScalef(0.25f, 0.4f, 0.25f);
    glutSolidSphere(0.2f, 8, 8);
    glPopMatrix();
    
    // Front left leg (strong)
    glColor3f(0.6f, 0.5f, 0.4f);
    glPushMatrix();
    glTranslatef(0.4f, -0.85f, 0.38f);
    glScalef(0.4f, 1.65f, 0.4f);
    glutSolidSphere(0.32f, 10, 10);
    glPopMatrix();
    
    // Front right leg
    glPushMatrix();
    glTranslatef(0.4f, -0.85f, -0.38f);
    glScalef(0.4f, 1.65f, 0.4f);
    glutSolidSphere(0.32f, 10, 10);
    glPopMatrix();
    
    // Back left leg (longer, sloped)
    glPushMatrix();
    glTranslatef(-0.7f, -0.65f, 0.38f);
    glScalef(0.35f, 1.4f, 0.35f);
    glutSolidSphere(0.3f, 10, 10);
    glPopMatrix();
    
    // Back right leg
    glPushMatrix();
    glTranslatef(-0.7f, -0.65f, -0.38f);
    glScalef(0.35f, 1.4f, 0.35f);
    glutSolidSphere(0.3f, 10, 10);
    glPopMatrix();
    
    // Tail (stiff, short)
    glColor3f(0.6f, 0.5f, 0.4f);
    glPushMatrix();
    glTranslatef(-1.2f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.16f, 0.8f, 8, 8);
    glPopMatrix();
    
    glPopMatrix();
}

void drawRhino(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Front body (massive)
    glColor3f(0.48f, 0.43f, 0.38f);
    glPushMatrix();
    glTranslatef(0.3f, 0.0f, 0.0f);
    glScalef(1.6f, 1.35f, 1.05f);
    glutSolidSphere(0.7f, 16, 16);
    glPopMatrix();
    
    // Back body (tapering)
    glColor3f(0.5f, 0.45f, 0.4f);
    glPushMatrix();
    glTranslatef(-0.8f, -0.1f, 0.0f);
    glScalef(1.35f, 1.2f, 0.95f);
    glutSolidSphere(0.6f, 14, 14);
    glPopMatrix();
    
    // Head (blocky, powerful)
    glColor3f(0.52f, 0.47f, 0.42f);
    glPushMatrix();
    glTranslatef(1.4f, 0.4f, 0.0f);
    glScalef(1.1f, 1.0f, 1.0f);
    glutSolidSphere(0.52f, 14, 14);
    
    // Snout
    glColor3f(0.56f, 0.51f, 0.46f);
    glPushMatrix();
    glTranslatef(0.5f, -0.2f, 0.0f);
    glScalef(0.85f, 0.65f, 0.75f);
    glutSolidSphere(0.35f, 10, 10);
    glPopMatrix();
    
    // Eyes (small)
    glColor3f(0.2f, 0.15f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.28f);
    glutSolidSphere(0.06f, 6, 6);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, -0.28f);
    glutSolidSphere(0.06f, 6, 6);
    glPopMatrix();
    glPopMatrix();
    
    // Primary horn (large)
    glColor3f(0.35f, 0.3f, 0.25f);
    glPushMatrix();
    glTranslatef(2.05f, 0.65f, 0.0f);
    glRotatef(12.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.22f, 1.2f, 10, 10);
    glPopMatrix();
    
    // Secondary horn/bump
    glColor3f(0.4f, 0.35f, 0.3f);
    glPushMatrix();
    glTranslatef(1.65f, 0.0f, 0.0f);
    glRotatef(-25.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.15f, 0.7f, 8, 8);
    glPopMatrix();
    
    // Front left leg (thick, columnar)
    glColor3f(0.46f, 0.41f, 0.36f);
    glPushMatrix();
    glTranslatef(0.6f, -0.95f, 0.5f);
    glScalef(0.52f, 1.95f, 0.52f);
    glutSolidSphere(0.42f, 12, 12);
    glPopMatrix();
    
    // Front right leg
    glPushMatrix();
    glTranslatef(0.6f, -0.95f, -0.5f);
    glScalef(0.52f, 1.95f, 0.52f);
    glutSolidSphere(0.42f, 12, 12);
    glPopMatrix();
    
    // Back left leg
    glPushMatrix();
    glTranslatef(-0.9f, -0.95f, 0.55f);
    glScalef(0.5f, 1.9f, 0.5f);
    glutSolidSphere(0.4f, 12, 12);
    glPopMatrix();
    
    // Back right leg
    glPushMatrix();
    glTranslatef(-0.9f, -0.95f, -0.55f);
    glScalef(0.5f, 1.9f, 0.5f);
    glutSolidSphere(0.4f, 12, 12);
    glPopMatrix();
    
    // Wrinkle texture (dark patches)
    glColor3f(0.42f, 0.37f, 0.32f);
    glBegin(GL_LINES);
    glLineWidth(0.8f);
    for (int i = 0; i < 6; i++) {
        float yOffset = 0.2f - (i * 0.15f);
        glVertex3f(-0.2f, yOffset, -0.3f);
        glVertex3f(-0.2f, yOffset, 0.3f);
    }
    glLineWidth(1.0f);
    glEnd();
    
    // Tail
    glColor3f(0.46f, 0.41f, 0.36f);
    glPushMatrix();
    glTranslatef(-1.8f, -0.2f, 0.0f);
    float tailSway = sinf(globalTime * 3.0f) * 8.0f;
    glRotatef(tailSway, 0.0f, 0.0f, 1.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.18f, 1.3f, 8, 8);
    glPopMatrix();
    
    glPopMatrix();
}

void drawGiraffe(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Body (rectangular)
    glColor3f(0.78f, 0.63f, 0.38f);
    glPushMatrix();
    glTranslatef(-0.1f, 0.3f, 0.0f);
    glScalef(1.4f, 1.1f, 0.8f);
    glutSolidSphere(0.58f, 14, 14);
    glPopMatrix();
    
    // Neck (very long and thin)
    glColor3f(0.8f, 0.65f, 0.4f);
    glPushMatrix();
    glTranslatef(0.5f, 2.2f, 0.0f);
    glScalef(0.4f, 2.4f, 0.4f);
    glutSolidSphere(0.3f, 12, 12);
    
    // Neck spots
    glColor3f(0.6f, 0.45f, 0.2f);
    for (int i = 0; i < 8; i++) {
        float spotY = -0.8f + (i * 0.25f);
        glPushMatrix();
        glTranslatef(0.0f, spotY, 0.0f);
        glutSolidSphere(0.12f, 6, 6);
        glPopMatrix();
    }
    glPopMatrix();
    
    // Head (small, narrow)
    glColor3f(0.84f, 0.69f, 0.44f);
    glPushMatrix();
    glTranslatef(0.55f, 4.8f, 0.0f);
    glScalef(0.7f, 0.75f, 0.65f);
    glutSolidSphere(0.32f, 10, 10);
    
    // Eyes (large, gentle)
    glColor3f(0.2f, 0.15f, 0.1f);
    glPushMatrix();
    glTranslatef(-0.1f, 0.05f, 0.25f);
    glutSolidSphere(0.07f, 6, 6);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.1f, 0.05f, -0.25f);
    glutSolidSphere(0.07f, 6, 6);
    glPopMatrix();
    glPopMatrix();
    
    // Main ossicones (horns)
    glColor3f(0.68f, 0.53f, 0.28f);
    glPushMatrix();
    glTranslatef(0.35f, 5.35f, 0.18f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.12f, 0.55f, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.35f, 5.35f, -0.18f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.12f, 0.55f, 8, 8);
    glPopMatrix();
    
    // Secondary ossicones (thinner)
    glColor3f(0.65f, 0.5f, 0.25f);
    glPushMatrix();
    glTranslatef(0.5f, 5.15f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.08f, 0.35f, 6, 6);
    glPopMatrix();
    
    // Front left leg (very long and thin)
    glColor3f(0.72f, 0.57f, 0.32f);
    glPushMatrix();
    glTranslatef(0.2f, -0.5f, 0.35f);
    glScalef(0.28f, 2.35f, 0.28f);
    glutSolidSphere(0.24f, 10, 10);
    glPopMatrix();
    
    // Front right leg
    glPushMatrix();
    glTranslatef(0.2f, -0.5f, -0.35f);
    glScalef(0.28f, 2.35f, 0.28f);
    glutSolidSphere(0.24f, 10, 10);
    glPopMatrix();
    
    // Back left leg
    glPushMatrix();
    glTranslatef(-0.8f, -0.5f, 0.35f);
    glScalef(0.28f, 2.3f, 0.28f);
    glutSolidSphere(0.24f, 10, 10);
    glPopMatrix();
    
    // Back right leg
    glPushMatrix();
    glTranslatef(-0.8f, -0.5f, -0.35f);
    glScalef(0.28f, 2.3f, 0.28f);
    glutSolidSphere(0.24f, 10, 10);
    glPopMatrix();
    
    // Body spots
    glColor3f(0.6f, 0.45f, 0.2f);
    for (int i = 0; i < 6; i++) {
        float spotX = -0.3f + (i % 3) * 0.35f;
        float spotZ = (i / 3) * 0.35f - 0.18f;
        glPushMatrix();
        glTranslatef(spotX, 0.35f, spotZ);
        glutSolidSphere(0.18f, 8, 8);
        glPopMatrix();
    }
    
    // Mane (dark line along neck)
    glColor3f(0.4f, 0.3f, 0.15f);
    glBegin(GL_LINE_STRIP);
    glLineWidth(1.5f);
    for (float h = 0.5f; h <= 2.2f; h += 0.2f) {
        glVertex3f(0.75f, h, 0.0f);
    }
    glLineWidth(1.0f);
    glEnd();
    
    // Tail (long with tuft)
    glColor3f(0.72f, 0.57f, 0.32f);
    glPushMatrix();
    glTranslatef(-1.3f, 0.0f, 0.0f);
    float tailSway = sinf(globalTime * 3.5f) * 12.0f;
    glRotatef(tailSway, 0.0f, 0.0f, 1.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.15f, 1.8f, 8, 8);
    glPopMatrix();
    
    glPopMatrix();
}

void drawAntelope(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    
    // Chest (sleek)
    glColor3f(0.72f, 0.62f, 0.42f);
    glPushMatrix();
    glTranslatef(0.15f, 0.1f, 0.0f);
    glScalef(1.1f, 0.95f, 0.7f);
    glutSolidSphere(0.48f, 12, 12);
    glPopMatrix();
    
    // Back body (thinner)
    glColor3f(0.7f, 0.6f, 0.4f);
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, 0.0f);
    glScalef(1.2f, 0.8f, 0.65f);
    glutSolidSphere(0.45f, 10, 10);
    glPopMatrix();
    
    // Head (graceful, narrow)
    glColor3f(0.74f, 0.64f, 0.44f);
    glPushMatrix();
    glTranslatef(1.0f, 0.5f, 0.0f);
    glScalef(0.85f, 0.8f, 0.72f);
    glutSolidSphere(0.37f, 10, 10);
    
    // Muzzle
    glColor3f(0.76f, 0.66f, 0.46f);
    glPushMatrix();
    glTranslatef(0.35f, -0.05f, 0.0f);
    glScalef(0.65f, 0.55f, 0.6f);
    glutSolidSphere(0.28f, 8, 8);
    glPopMatrix();
    
    // Eyes (large, alert)
    glColor3f(0.2f, 0.15f, 0.1f);
    glPushMatrix();
    glTranslatef(0.1f, 0.15f, 0.22f);
    glutSolidSphere(0.08f, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.1f, 0.15f, -0.22f);
    glutSolidSphere(0.08f, 8, 8);
    glPopMatrix();
    glPopMatrix();
    
    // Ears (pointed, alert)
    glColor3f(0.68f, 0.58f, 0.38f);
    glPushMatrix();
    glTranslatef(1.2f, 0.95f, 0.2f);
    glScalef(0.25f, 0.5f, 0.22f);
    glutSolidSphere(0.2f, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.2f, 0.95f, -0.2f);
    glScalef(0.25f, 0.5f, 0.22f);
    glutSolidSphere(0.2f, 8, 8);
    glPopMatrix();
    
    // Horns (V-shaped, sharp)
    glColor3f(0.45f, 0.35f, 0.15f);
    glPushMatrix();
    glTranslatef(1.15f, 0.9f, 0.16f);
    glRotatef(35.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.09f, 0.75f, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.15f, 0.9f, -0.16f);
    glRotatef(-35.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.09f, 0.75f, 8, 8);
    glPopMatrix();
    
    // Front left leg (very thin, elegant)
    glColor3f(0.68f, 0.58f, 0.38f);
    glPushMatrix();
    glTranslatef(0.25f, -0.7f, 0.28f);
    glScalef(0.26f, 1.45f, 0.26f);
    glutSolidSphere(0.22f, 8, 8);
    glPopMatrix();
    
    // Front right leg
    glPushMatrix();
    glTranslatef(0.25f, -0.7f, -0.28f);
    glScalef(0.26f, 1.45f, 0.26f);
    glutSolidSphere(0.22f, 8, 8);
    glPopMatrix();
    
    // Back left leg
    glPushMatrix();
    glTranslatef(-0.65f, -0.7f, 0.28f);
    glScalef(0.26f, 1.42f, 0.26f);
    glutSolidSphere(0.22f, 8, 8);
    glPopMatrix();
    
    // Back right leg
    glPushMatrix();
    glTranslatef(-0.65f, -0.7f, -0.28f);
    glScalef(0.26f, 1.42f, 0.26f);
    glutSolidSphere(0.22f, 8, 8);
    glPopMatrix();
    
    // White belly stripe
    glColor3f(0.95f, 0.95f, 0.9f);
    glBegin(GL_QUADS);
    glVertex3f(-0.3f, 0.1f, -0.08f);
    glVertex3f(-0.3f, 0.1f, 0.08f);
    glVertex3f(0.8f, 0.1f, 0.08f);
    glVertex3f(0.8f, 0.1f, -0.08f);
    glEnd();
    
    // Tail (thin, tufted)
    glColor3f(0.6f, 0.5f, 0.3f);
    glPushMatrix();
    glTranslatef(-1.25f, 0.05f, 0.0f);
    float tailSway = sinf(globalTime * 6.0f) * 15.0f;
    glRotatef(tailSway, 0.0f, 0.0f, 1.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.12f, 1.0f, 6, 6);
    glPopMatrix();
    
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(0, 8, 28,    0, 4, 0,    0, 1, 0);
    
    drawSky();
    drawGround();
    
    // Draw trees scattered around
    drawTree(-30.0f, 20.0f, 1.5f);
    drawTree(25.0f, -25.0f, 1.4f);
    drawTree(-15.0f, -30.0f, 1.3f);
    drawTree(35.0f, 15.0f, 1.2f);
    drawTree(-40.0f, -15.0f, 1.6f);
    drawTree(20.0f, 30.0f, 1.2f);
    
    // Draw all animals
    for (auto& animal : animals) {
        animal.x += animal.speed;
        if (animal.x > 25.0f) animal.x = -25.0f;
        
        switch (animal.type) {
            case 0:  // Leopard
                drawLeopard(animal.x, animal.z, animal.scale);
                break;
            case 1:  // Hyena
                drawHyena(animal.x, animal.z, animal.scale);
                break;
            case 2:  // Rhino
                drawRhino(animal.x, animal.z, animal.scale);
                break;
            case 3:  // Giraffe
                drawGiraffe(animal.x, animal.z, animal.scale);
                break;
            case 4:  // Antelope
                drawAntelope(animal.x, animal.z, animal.scale);
                break;
        }
    }
    
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
    glColor3f(0.3f, 0.5f, 0.2f);
    glRasterPos2f(200, 50);
    const char* title = "Chapter 16: Mammalian Rise - African Savanna";
    for (const char* c = title; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
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
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
}

void timer(int v) {
    globalTime += 0.016f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_position[] = {20.0f, 15.0f, 10.0f, 0.0f};
    GLfloat light_ambient[] = {0.6f, 0.6f, 0.5f, 1.0f};
    GLfloat light_diffuse[] = {0.9f, 0.85f, 0.7f, 1.0f};
    GLfloat light_specular[] = {0.8f, 0.8f, 0.7f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void init() {
    glClearColor(0.7f, 0.9f, 1.0f, 1.0f);  // Light blue background
    glEnable(GL_DEPTH_TEST);
    setupLighting();
    initScene();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Chapter 16: Mammalian Rise");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}
