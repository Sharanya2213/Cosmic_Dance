#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

float globalTime = 0.0f;
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 1000;

// Big Bang Particle System
struct BBParticle {
    float px, py, pz;
    float vx, vy, vz;
    float r, g, b, a;
    float lifetime;
};

std::vector<BBParticle> bbParticles;

// Initialize Big Bang particles
void initBigBangParticles() {
    bbParticles.clear();
    
    // Create 80,000 particles expanding from center
    for (int i = 0; i < 80000; ++i) {
        BBParticle p;
        
        // Random direction (spherical distribution)
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float phi = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 0.3f + (rand() % 100) / 150.0f;
        
        p.px = 0.0f;
        p.py = 5.0f;
        p.pz = -24.8f;
        
        p.vx = sinf(phi) * cosf(angle) * speed;
        p.vy = sinf(phi) * sinf(angle) * speed;
        p.vz = cosf(phi) * speed * 0.5f;
        
        // Temperature spectrum: white → yellow → orange → blue-white
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
        
        p.a = 0.8f;
        p.lifetime = 0.0f;
        
        bbParticles.push_back(p);
    }
}

// Update Big Bang particles with physics
void updateBigBangParticles() {
    float bangTime = globalTime;
    if (bangTime < 0.5f) return;  // Before big bang reveal
    
    float actualBangTime = bangTime - 0.5f;
    
    for (size_t i = 0; i < bbParticles.size(); ++i) {
        auto& p = bbParticles[i];
        p.lifetime = actualBangTime;
        
        // EXPLOSIVE LAUNCH PHASE (first 0.5s)
        if (actualBangTime < 0.5f) {
            float expFactor = 1.0f - actualBangTime * 0.1f;
            p.vx *= (0.95f + expFactor * 0.05f);
            p.vy *= (0.95f + expFactor * 0.05f);
            p.vz *= (0.95f + expFactor * 0.05f);
        } 
        // DECELERATION & COOLING (0.5s - 3s)
        else if (actualBangTime < 3.0f) {
            float cool = (actualBangTime - 0.5f) / 2.5f;
            p.r = p.r * (1.0f - cool * 0.3f);
            p.g = p.g * (1.0f - cool * 0.5f);
            p.b = p.b * (1.0f - cool * 0.8f);
            p.a = 0.7f - cool * 0.3f;
            p.vx *= (1.0f - actualBangTime * 0.05f);
            p.vy *= (1.0f - actualBangTime * 0.05f);
            p.vz *= (1.0f - actualBangTime * 0.05f);
        }
        // STABILIZATION (3s - 10s)
        else if (actualBangTime < 10.0f) {
            p.r = 0.8f; p.g = 0.2f; p.b = 0.1f;
            p.a = std::max(0.3f, 0.4f * (1.0f - (actualBangTime - 3.0f) / 7.0f));
            p.vx *= 0.98f;
            p.vy *= 0.98f;
            p.vz *= 0.98f;
        }
        // FINAL FADE (10s+)
        else {
            float fadeProgress = (actualBangTime - 10.0f) / 5.0f;
            if (fadeProgress > 1.0f) fadeProgress = 1.0f;
            p.a = 0.3f * (1.0f - fadeProgress);
        }
        
        p.px += p.vx * 0.016f;
        p.py += p.vy * 0.016f;
        p.pz += p.vz * 0.016f;
        
        if (p.a < 0.0f) p.a = 0.0f;
    }
}

void drawText3D(const char* text, float x, float y, float z) {
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 1.0f, 1.0f);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

void drawText3DLarge(const char* text, float x, float y, float z) {
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 1.0f, 1.0f);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

void drawCircle(float radius, int segments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex3f(radius * cosf(angle), radius * sinf(angle), 0.0f);
    }
    glEnd();
}

void drawOrbitalSymbol() {
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 1.0f, 1.0f);
    glLineWidth(2.5f);
    
    // Three orbital rings
    drawCircle(1.8f, 64);
    
    glPushMatrix();
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    drawCircle(1.4f, 64);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
    drawCircle(1.0f, 64);
    glPopMatrix();
    
    // Central rotating element
    glPushMatrix();
    glRotatef(globalTime * 30.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(0.0f, -0.5f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glEnd();
    glPopMatrix();
    
    // Center dot
    glColor3f(0.0f, 1.0f, 1.0f);
    glutSolidSphere(0.2f, 16, 16);
    
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

void drawButton(float x, float y, float r, float g, float b, bool lit) {
    glDisable(GL_LIGHTING);
    
    // Outer button frame (darker shading for depth)
    glColor3f(r * 0.5f, g * 0.5f, b * 0.5f);
    glBegin(GL_QUADS);
    glVertex3f(x - 0.25f, y - 0.25f, 0.0f);
    glVertex3f(x + 0.25f, y - 0.25f, 0.0f);
    glVertex3f(x + 0.25f, y + 0.25f, 0.0f);
    glVertex3f(x - 0.25f, y + 0.25f, 0.0f);
    glEnd();
    
    // Button background with brightness variation
    float brightness = lit ? 1.2f : 1.0f;
    glColor3f(r * brightness, g * brightness, b * brightness);
    glBegin(GL_QUADS);
    glVertex3f(x - 0.22f, y - 0.22f, 0.0f);
    glVertex3f(x + 0.22f, y - 0.22f, 0.0f);
    glVertex3f(x + 0.22f, y + 0.22f, 0.0f);
    glVertex3f(x - 0.22f, y + 0.22f, 0.0f);
    glEnd();
    
    // Button highlight (upper left)
    glColor3f(r * 1.3f, g * 1.3f, b * 1.3f);
    glBegin(GL_TRIANGLES);
    glVertex3f(x - 0.22f, y + 0.22f, 0.0f);
    glVertex3f(x - 0.05f, y + 0.22f, 0.0f);
    glVertex3f(x - 0.22f, y + 0.05f, 0.0f);
    glEnd();
    
    // Button border (cyan/white)
    glColor3f(0.2f, 1.0f, 1.0f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x - 0.22f, y - 0.22f, 0.0f);
    glVertex3f(x + 0.22f, y - 0.22f, 0.0f);
    glVertex3f(x + 0.22f, y + 0.22f, 0.0f);
    glVertex3f(x - 0.22f, y + 0.22f, 0.0f);
    glEnd();
    glLineWidth(1.0f);
    
    glEnable(GL_LIGHTING);
}

void drawControlConsole() {
    glDisable(GL_LIGHTING);
    
    // Console background panel - NASA dark blue
    glColor3f(0.0f, 0.05f, 0.15f);
    glBegin(GL_QUADS);
    glVertex3f(-14.0f, -6.0f, -25.0f);
    glVertex3f(14.0f, -6.0f, -25.0f);
    glVertex3f(14.0f, -1.5f, -25.0f);
    glVertex3f(-14.0f, -1.5f, -25.0f);
    glEnd();
    
    // Console border - bright cyan NASA style
    glColor3f(0.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-14.0f, -6.0f, -25.0f);
    glVertex3f(14.0f, -6.0f, -25.0f);
    glVertex3f(14.0f, -1.5f, -25.0f);
    glVertex3f(-14.0f, -1.5f, -25.0f);
    glEnd();
    
    // Internal dividers - cyan
    glColor3f(0.0f, 0.8f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-5.0f, -6.0f, -25.0f);
    glVertex3f(-5.0f, -1.5f, -25.0f);
    glVertex3f(5.0f, -6.0f, -25.0f);
    glVertex3f(5.0f, -1.5f, -25.0f);
    glEnd();
    glLineWidth(1.0f);
    
    // Button rows with more buttons
    float yPos = -3.5f;
    float xStart = -12.5f;
    float buttonSpacing = 0.7f;
    
    // Red buttons (5)
    for (int i = 0; i < 5; i++) {
        bool lit = sinf(globalTime * 2.0f + i) > 0.0f;
        drawButton(xStart + i * buttonSpacing, yPos, 1.0f, 0.0f, 0.0f, lit);
    }
    
    // Green buttons (5)
    for (int i = 0; i < 5; i++) {
        bool lit = sinf(globalTime * 2.5f + i * 0.5f) > 0.3f;
        drawButton(xStart + 4.2f + i * buttonSpacing, yPos, 0.0f, 1.0f, 0.0f, lit);
    }
    
    // Yellow buttons (5)
    for (int i = 0; i < 5; i++) {
        bool lit = cosf(globalTime * 1.8f + i) > 0.2f;
        drawButton(xStart + 8.4f + i * buttonSpacing, yPos, 1.0f, 1.0f, 0.0f, lit);
    }
    
    // Bottom row - blue and white buttons
    float yPos2 = -2.5f;
    for (int i = 0; i < 5; i++) {
        bool lit = sinf(globalTime * 2.2f + i) > 0.1f;
        drawButton(xStart + i * buttonSpacing, yPos2, 0.0f, 0.7f, 1.0f, lit);
    }
    
    for (int i = 0; i < 5; i++) {
        bool lit = cosf(globalTime * 2.0f + i * 0.7f) > 0.0f;
        drawButton(xStart + 4.2f + i * buttonSpacing, yPos2, 1.0f, 1.0f, 1.0f, lit);
    }
    
    // Data visualization graphs (3 panels) - bright cyan lines
    // Panel 1
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for (float x = -10.0f; x < -5.0f; x += 0.2f) {
        float barHeight = 0.5f + 0.3f * sinf(globalTime * 2.0f + x);
        glVertex3f(x, -5.5f + barHeight, -25.0f);
    }
    glEnd();
    
    // Panel 2 (centered)
    glBegin(GL_LINE_STRIP);
    for (float x = -0.5f; x < 4.5f; x += 0.2f) {
        float barHeight = 0.6f + 0.35f * sinf(globalTime * 1.8f + x * 0.5f);
        glVertex3f(x, -5.3f + barHeight, -25.0f);
    }
    glEnd();
    
    // Panel 3
    glBegin(GL_LINE_STRIP);
    for (float x = 6.0f; x < 11.0f; x += 0.2f) {
        float barHeight = 0.4f + 0.4f * cosf(globalTime * 2.3f + x);
        glVertex3f(x, -5.4f + barHeight, -25.0f);
    }
    glEnd();
    
    // Draw "BIG BANG" text on console panel
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    
    // Centered text on console
    float consoleTextCenterX = 0.0f;
    float consoleTextCenterY = -4.0f;
    float consoleLetterSpacing = 0.5f;
    float consoleStartX = consoleTextCenterX - (3.0f * consoleLetterSpacing);
    
    glBegin(GL_LINES);
    
    // B
    float cx = consoleStartX;
    glVertex3f(cx, consoleTextCenterY - 0.3f, -25.0f); glVertex3f(cx, consoleTextCenterY + 0.3f, -25.0f);
    glVertex3f(cx, consoleTextCenterY + 0.3f, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY + 0.3f, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY + 0.3f, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY, -25.0f); glVertex3f(cx, consoleTextCenterY, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY - 0.3f, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY - 0.3f, -25.0f); glVertex3f(cx, consoleTextCenterY - 0.3f, -25.0f);
    
    // I
    cx = consoleStartX + consoleLetterSpacing;
    glVertex3f(cx + 0.04f, consoleTextCenterY - 0.3f, -25.0f); glVertex3f(cx + 0.04f, consoleTextCenterY + 0.3f, -25.0f);
    
    // G
    cx = consoleStartX + 2 * consoleLetterSpacing;
    glVertex3f(cx + 0.25f, consoleTextCenterY + 0.3f, -25.0f); glVertex3f(cx, consoleTextCenterY + 0.3f, -25.0f);
    glVertex3f(cx, consoleTextCenterY + 0.3f, -25.0f); glVertex3f(cx, consoleTextCenterY - 0.1f, -25.0f);
    glVertex3f(cx, consoleTextCenterY - 0.1f, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY - 0.1f, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY - 0.1f, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY - 0.3f, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY - 0.3f, -25.0f); glVertex3f(cx, consoleTextCenterY - 0.3f, -25.0f);
    
    // B
    cx = consoleStartX + 3.5f * consoleLetterSpacing;
    glVertex3f(cx, consoleTextCenterY - 0.3f, -25.0f); glVertex3f(cx, consoleTextCenterY + 0.3f, -25.0f);
    glVertex3f(cx, consoleTextCenterY + 0.3f, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY + 0.3f, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY + 0.3f, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY, -25.0f); glVertex3f(cx, consoleTextCenterY, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY - 0.3f, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY - 0.3f, -25.0f); glVertex3f(cx, consoleTextCenterY - 0.3f, -25.0f);
    
    // A
    cx = consoleStartX + 4.5f * consoleLetterSpacing;
    glVertex3f(cx, consoleTextCenterY - 0.3f, -25.0f); glVertex3f(cx + 0.125f, consoleTextCenterY + 0.3f, -25.0f);
    glVertex3f(cx + 0.125f, consoleTextCenterY + 0.3f, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY - 0.3f, -25.0f);
    glVertex3f(cx + 0.06f, consoleTextCenterY, -25.0f); glVertex3f(cx + 0.19f, consoleTextCenterY, -25.0f);
    
    // N
    cx = consoleStartX + 5.5f * consoleLetterSpacing;
    glVertex3f(cx, consoleTextCenterY - 0.3f, -25.0f); glVertex3f(cx, consoleTextCenterY + 0.3f, -25.0f);
    glVertex3f(cx, consoleTextCenterY + 0.3f, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY - 0.3f, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY - 0.3f, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY + 0.3f, -25.0f);
    
    // G
    cx = consoleStartX + 6.5f * consoleLetterSpacing;
    glVertex3f(cx + 0.25f, consoleTextCenterY + 0.3f, -25.0f); glVertex3f(cx, consoleTextCenterY + 0.3f, -25.0f);
    glVertex3f(cx, consoleTextCenterY + 0.3f, -25.0f); glVertex3f(cx, consoleTextCenterY - 0.1f, -25.0f);
    glVertex3f(cx, consoleTextCenterY - 0.1f, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY - 0.1f, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY - 0.1f, -25.0f); glVertex3f(cx + 0.25f, consoleTextCenterY - 0.3f, -25.0f);
    glVertex3f(cx + 0.25f, consoleTextCenterY - 0.3f, -25.0f); glVertex3f(cx, consoleTextCenterY - 0.3f, -25.0f);
    
    glEnd();
    glLineWidth(1.0f);
    
    glEnable(GL_LIGHTING);
}

void drawNarrativeText() {
    // Display narrative text overlay at the top of screen
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    float textTime = globalTime;
    float fadeIn = std::min(textTime / 0.5f, 1.0f);  // Fade in quickly
    float fadeOut = 0.0f;
    if (textTime > 1.0f) {
        fadeOut = std::min((textTime - 1.0f) / 0.5f, 1.0f);
        fadeIn = 1.0f - fadeOut;
    }
    
    if (fadeIn > 0.01f) {
        glColor4f(0.0f, 1.0f, 1.0f, fadeIn * 0.8f);
        
        // Main title only
        glRasterPos2f(300.0f, 950.0f);
        const char* title = "CHAPTER 17: HUMANITY OBSERVES THE BEGINNING";
        for (int i = 0; title[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title[i]);
        }
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

void drawSaganQuote() {
    // Display Carl Sagan quote centered on black screen
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    float quoteTime = globalTime - 2.0f;
    if (quoteTime >= 0.0f) {
        // Quote appears at 10s and stays visible for 2 seconds, then fades
        float fadeIn = std::min(quoteTime / 0.3f, 1.0f);  // Quick fade in
        float fadeOut = 0.0f;
        
        // Fade out in the last 0.5 seconds
        if (quoteTime > 1.5f) {
            fadeOut = std::min((quoteTime - 1.5f) / 0.5f, 1.0f);
            fadeIn = 1.0f - fadeOut;
        }
        
        if (fadeIn > 0.01f) {
            glColor4f(1.0f, 1.0f, 1.0f, fadeIn);
            
            // Quote line 1 - centered
            const char* quote = "\"We are a way for the universe to know itself.\"";
            int quoteWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)quote);
            float quoteXPos = (WINDOW_WIDTH - quoteWidth) / 2.0f;
            
            glRasterPos2f(quoteXPos, 540.0f);
            for (int i = 0; quote[i] != '\0'; i++) {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, quote[i]);
            }
            
            // Author line - centered
            const char* author = "— Carl Sagan";
            int authorWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (unsigned char*)author);
            float authorXPos = (WINDOW_WIDTH - authorWidth) / 2.0f;
            
            glRasterPos2f(authorXPos, 480.0f);
            for (int i = 0; author[i] != '\0'; i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, author[i]);
            }
        }
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

void drawSideScreen(float x, float z) {
    glDisable(GL_LIGHTING);
    
    // Screen frame (bright cyan border - NASA style)
    glColor3f(0.0f, 1.0f, 1.0f);
    glLineWidth(2.5f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x - 2.0f, 3.0f, z);
    glVertex3f(x + 2.0f, 3.0f, z);
    glVertex3f(x + 2.0f, 8.0f, z);
    glVertex3f(x - 2.0f, 8.0f, z);
    glEnd();
    glLineWidth(1.0f);
    
    // Screen background - NASA dark blue
    glColor3f(0.0f, 0.05f, 0.15f);
    glBegin(GL_QUADS);
    glVertex3f(x - 2.0f, 3.0f, z);
    glVertex3f(x + 2.0f, 3.0f, z);
    glVertex3f(x + 2.0f, 8.0f, z);
    glVertex3f(x - 2.0f, 8.0f, z);
    glEnd();
    
    // Simple grid pattern on screen - bright cyan
    glColor3f(0.0f, 1.0f, 1.0f);
    for (float y = 3.5f; y < 8.0f; y += 0.5f) {
        glBegin(GL_LINES);
        glVertex3f(x - 1.8f, y, z);
        glVertex3f(x + 1.8f, y, z);
        glEnd();
    }
    
    for (float xi = x - 1.8f; xi < x + 2.0f; xi += 0.5f) {
        glBegin(GL_LINES);
        glVertex3f(xi, 3.0f, z);
        glVertex3f(xi, 8.0f, z);
        glEnd();
    }
    
    glEnable(GL_LIGHTING);
}

void drawMainScreen() {
    glDisable(GL_LIGHTING);
    
    // Main screen background - NASA dark blue/black
    glColor3f(0.0f, 0.05f, 0.15f);
    glBegin(GL_QUADS);
    glVertex3f(-6.0f, 0.0f, -25.0f);
    glVertex3f(6.0f, 0.0f, -25.0f);
    glVertex3f(6.0f, 10.0f, -25.0f);
    glVertex3f(-6.0f, 10.0f, -25.0f);
    glEnd();
    
    // Main screen border - bright cyan NASA style
    glColor3f(0.0f, 1.0f, 1.0f);
    glLineWidth(4.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-6.0f, 0.0f, -25.0f);
    glVertex3f(6.0f, 0.0f, -25.0f);
    glVertex3f(6.0f, 10.0f, -25.0f);
    glVertex3f(-6.0f, 10.0f, -25.0f);
    glEnd();
    glLineWidth(1.0f);
    
    // Draw Big Bang particles with full physics system
    float bangTime = globalTime - 0.5f;
    if (bangTime > 0.0f) {
        for (size_t i = 0; i < bbParticles.size(); ++i) {
            auto& p = bbParticles[i];
            glColor4f(p.r, p.g, p.b, p.a);
            glPointSize(2.0f);
            glBegin(GL_POINTS);
            glVertex3f(p.px, p.py, p.pz);
            glEnd();
        }
    }
    
    // Central white core
    float coreAlpha = 1.0f;
    if (bangTime > 5.0f) {
        coreAlpha = std::max(0.0f, 1.0f - (bangTime - 5.0f) / 5.0f);
    }
    glColor4f(1.0f, 1.0f, 1.0f, coreAlpha);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 5.0f, -24.8f);
    glEnd();
    
    glEnable(GL_LIGHTING);
}

void drawRoom() {
    glDisable(GL_LIGHTING);
    
    // Floor (very dark)
    glColor3f(0.0f, 0.02f, 0.05f);
    glBegin(GL_QUADS);
    glVertex3f(-30.0f, 0.0f, -30.0f);
    glVertex3f(30.0f, 0.0f, -30.0f);
    glVertex3f(30.0f, 0.0f, 30.0f);
    glVertex3f(-30.0f, 0.0f, 30.0f);
    glEnd();
    
    // Back wall
    glColor3f(0.0f, 0.03f, 0.08f);
    glBegin(GL_QUADS);
    glVertex3f(-30.0f, 0.0f, -30.0f);
    glVertex3f(30.0f, 0.0f, -30.0f);
    glVertex3f(30.0f, 20.0f, -30.0f);
    glVertex3f(-30.0f, 20.0f, -30.0f);
    glEnd();
    
    // Side walls
    glColor3f(0.0f, 0.04f, 0.10f);
    glBegin(GL_QUADS);
    glVertex3f(-30.0f, 0.0f, -30.0f);
    glVertex3f(-30.0f, 0.0f, 30.0f);
    glVertex3f(-30.0f, 20.0f, 30.0f);
    glVertex3f(-30.0f, 20.0f, -30.0f);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex3f(30.0f, 0.0f, -30.0f);
    glVertex3f(30.0f, 20.0f, -30.0f);
    glVertex3f(30.0f, 20.0f, 30.0f);
    glVertex3f(30.0f, 0.0f, 30.0f);
    glEnd();
    
    glEnable(GL_LIGHTING);
}

void drawHuman(float x, float z, float posture) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glColor3f(0.2f, 0.2f, 0.25f);
    
    // Body
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glRotatef(posture, 1.0f, 0.0f, 0.0f);
    glScalef(0.4f, 0.8f, 0.35f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Head
    glPushMatrix();
    glTranslatef(0.0f, 3.2f, -0.2f);
    glutSolidSphere(0.35f, 12, 12);
    glPopMatrix();
    
    // Arms
    glPushMatrix();
    glTranslatef(-0.35f, 2.5f, 0.0f);
    glScalef(0.2f, 0.7f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.35f, 2.5f, 0.0f);
    glScalef(0.2f, 0.7f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Legs
    glPushMatrix();
    glTranslatef(-0.2f, 0.8f, 0.0f);
    glScalef(0.2f, 0.8f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.2f, 0.8f, 0.0f);
    glScalef(0.2f, 0.8f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPopMatrix();
}

void display() {
    // Draw background black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // ONLY render 3D scene BEFORE fade is complete (before 2s)
    if (globalTime < 2.0f) {
        // Calculate fade for scene - smooth fade during final 1 second
        float fadeStartTime = 1.0f;
        float fadeEndTime = 2.0f;
        float sceneFadeAlpha = 1.0f;
        
        if (globalTime >= fadeStartTime) {
            float fadeDuration = fadeEndTime - fadeStartTime;
            float timeInFade = globalTime - fadeStartTime;
            sceneFadeAlpha = std::max(0.0f, 1.0f - (timeInFade / fadeDuration));
        }
        
        glLoadIdentity();
        
        // Update Big Bang particle system
        updateBigBangParticles();
        
        gluLookAt(0.0f, 6.0f, 18.0f, 0.0f, 4.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        
        GLfloat light_position[] = {0.0f, 10.0f, 5.0f, 0.0f};
        GLfloat light_ambient[] = {0.3f, 0.3f, 0.35f, 1.0f};
        GLfloat light_diffuse[] = {0.5f, 0.5f, 0.6f, 1.0f};
        GLfloat light_specular[] = {0.2f, 0.2f, 0.25f, 1.0f};
        
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        
        GLfloat material_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat material_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
        GLfloat material_specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
        GLfloat material_shininess[] = {32.0f};
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
        
        // Draw environment with fade
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glColor4f(0.0f, 0.0f, 0.0f, sceneFadeAlpha);
        drawRoom();
        glPopAttrib();
        
        // Draw side monitoring screens (left column)
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glColor4f(1.0f, 1.0f, 1.0f, sceneFadeAlpha);
        drawSideScreen(-16.0f, -20.0f);
        drawSideScreen(-16.0f, -15.0f);
        drawSideScreen(-16.0f, -10.0f);
        drawSideScreen(-16.0f, -5.0f);
        
        // Draw side monitoring screens (right column)
        drawSideScreen(16.0f, -20.0f);
        drawSideScreen(16.0f, -15.0f);
        drawSideScreen(16.0f, -10.0f);
        drawSideScreen(16.0f, -5.0f);
        
        // Draw secondary screens (back center area)
        drawSideScreen(-7.0f, -23.0f);
        drawSideScreen(7.0f, -23.0f);
        glPopAttrib();
        
        // Draw main central display
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glColor4f(1.0f, 1.0f, 1.0f, sceneFadeAlpha);
        drawMainScreen();
        glPopAttrib();
        
        // Draw control console at bottom
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glColor4f(1.0f, 1.0f, 1.0f, sceneFadeAlpha);
        drawControlConsole();
        glPopAttrib();
        
        // Draw observing humans
        if (sceneFadeAlpha > 0.01f) {
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glColor4f(0.2f * sceneFadeAlpha, 0.2f * sceneFadeAlpha, 0.25f * sceneFadeAlpha, sceneFadeAlpha);
            float posture = sinf(globalTime * 0.5f) * 5.0f;
            drawHuman(-8.0f, 8.0f, 10.0f + posture);
            drawHuman(-3.0f, 9.0f, 12.0f + posture);
            drawHuman(2.0f, 9.5f, 11.0f + posture);
            drawHuman(6.0f, 8.5f, 13.0f + posture);
            drawHuman(10.0f, 9.0f, 12.0f + posture);
            glPopAttrib();
        }
        
        glEnable(GL_LIGHTING);
        
        // Draw narrative text overlay on top
        drawNarrativeText();
    }
    
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
        
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
    }
    
    // Draw Carl Sagan quote on black screen (after 3.5s)
    drawSaganQuote();
    
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    globalTime += 0.016f;
    
    // Auto-exit after 30 seconds for continuous playback
    if (globalTime >= 30.0f) {
        exit(0);
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void init() {
    glClearColor(0.0f, 0.01f, 0.02f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Initialize Big Bang particle system
    initBigBangParticles();
    
    printf("Initialized Chapter 17: Humanity Observes the Big Bang\n");
    printf("Features: Advanced Big Bang physics, NASA control center, narrative timeline\n");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ALPHA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Chapter 17: NASA Control Center");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}
