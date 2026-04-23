#include <GL/glut.h>
#include <windows.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

// ============================================================================
// SIMPLE MATH VECTORS (NO GLM)
// ============================================================================

struct Vec3 {
    float x, y, z;
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    
    Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    
    Vec3 operator*(float s) const {
        return Vec3(x * s, y * s, z * s);
    }
    
    Vec3& operator+=(const Vec3& v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    
    Vec3& operator*=(float s) {
        x *= s; y *= s; z *= s;
        return *this;
    }
    
    float dot(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    
    float length() const {
        return sqrt(x * x + y * y + z * z);
    }
    
    Vec3 normalize() const {
        float len = length();
        if (len < 0.0001f) return Vec3(0, 0, 0);
        return Vec3(x / len, y / len, z / len);
    }
};

struct Vec4 {
    float x, y, z, w;
    Vec4() : x(0), y(0), z(0), w(1) {}
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

// ============================================================================
// SHADER FUNCTIONS (CLASSIC OPENGL 1.2)
// ============================================================================

// Use function pointers without extension types - classic OpenGL
typedef unsigned int (*GLCREATESHADERF)(unsigned int type);
typedef void (*GLSHADERSOURCEF)(unsigned int, int, const char**, const int*);
typedef void (*GLCOMPILESHADERF)(unsigned int);
typedef void (*GLGETSHADERIVF)(unsigned int, unsigned int, int*);
typedef void (*GLGETSHADERINFOLOGF)(unsigned int, int, int*, char*);
typedef unsigned int (*GLCREATEPROGRAMF)(void);
typedef void (*GLATTACHSHADERF)(unsigned int, unsigned int);
typedef void (*GILLINKPROGRAMF)(unsigned int);
typedef void (*GLGETPROGRAMIVF)(unsigned int, unsigned int, int*);
typedef void (*GLGETPROGRAMINFOLOGF)(unsigned int, int, int*, char*);
typedef void (*GLUSEPROGRAMF)(unsigned int);

static GLCREATESHADERF glCreateShader = NULL;
static GLSHADERSOURCEF glShaderSource = NULL;
static GLCOMPILESHADERF glCompileShader = NULL;
static GLGETSHADERIVF glGetShaderiv = NULL;
static GLGETSHADERINFOLOGF glGetShaderInfoLog = NULL;
static GLCREATEPROGRAMF glCreateProgram = NULL;
static GLATTACHSHADERF glAttachShader = NULL;
static GILLINKPROGRAMF glLinkProgram = NULL;
static GLGETPROGRAMIVF glGetProgramiv = NULL;
static GLGETPROGRAMINFOLOGF glGetProgramInfoLog = NULL;
static GLUSEPROGRAMF glUseProgram = NULL;

#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84

void initShaderFunctions() {
    glCreateShader = (GLCREATESHADERF)wglGetProcAddress("glCreateShader");
    glShaderSource = (GLSHADERSOURCEF)wglGetProcAddress("glShaderSource");
    glCompileShader = (GLCOMPILESHADERF)wglGetProcAddress("glCompileShader");
    glGetShaderiv = (GLGETSHADERIVF)wglGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (GLGETSHADERINFOLOGF)wglGetProcAddress("glGetShaderInfoLog");
    glCreateProgram = (GLCREATEPROGRAMF)wglGetProcAddress("glCreateProgram");
    glAttachShader = (GLATTACHSHADERF)wglGetProcAddress("glAttachShader");
    glLinkProgram = (GILLINKPROGRAMF)wglGetProcAddress("glLinkProgram");
    glGetProgramiv = (GLGETPROGRAMIVF)wglGetProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (GLGETPROGRAMINFOLOGF)wglGetProcAddress("glGetProgramInfoLog");
    glUseProgram = (GLUSEPROGRAMF)wglGetProcAddress("glUseProgram");
}

GLuint compileShader(GLenum type, const char* source) {
    if (!glCreateShader) return 0;
    
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    int compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::cerr << "Shader compile error: " << log << std::endl;
    }
    return shader;
}

GLuint createProgram(const char* vertexSrc, const char* fragmentSrc) {
    if (!glCreateProgram) return 0;
    
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    int linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[512];
        glGetProgramInfoLog(program, 512, NULL, log);
        std::cerr << "Program link error: " << log << std::endl;
    }
    return program;
}

// ============================================================================
// GLOBAL CONSTANTS & VARIABLES
// ============================================================================

const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 1000;
const float STORY_DURATION = 25.0f;
const float TARGET_SCENE_DURATION = 18.75f;
const float TIME_SCALE = STORY_DURATION / TARGET_SCENE_DURATION;
const int NUM_PARTICLES = 100000;  // Reduced from 1M for CPU-GPU balance

float globalTime = 0.0f;
float deltaTime = 0.016f;

// Scene selection
int currentScene = 1;  // 1 = Chaos, 2 = Big Bang
float sceneTransitionTime = 0.0f;

// Camera
float cameraX = 0.0f, cameraY = 0.5f, cameraZ = 3.0f;

// ============================================================================
// PARTICLE STRUCTURE
// ============================================================================

struct Particle {
    Vec3 position;
    Vec3 velocity;
    Vec3 color;
    float alpha;
    float lifetime;
    int type;
};

std::vector<Particle> particles;

// ============================================================================
// BIG BANG SCENE
// ============================================================================

// ============================================================================
// BIG BANG INITIALIZATION
// ============================================================================

void initBigBangScene() {
    particles.clear();
    
    // Initialize primordial plasma particles
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        Particle p;
        
        // Launch from center outward - MUCH FASTER for violent burst
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float phi = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 2.0f + (rand() % 300) / 100.0f;  // 2.0-5.0 units/sec (was 0.5-1.5)
        
        p.position = Vec3(0.0f, 0.0f, 0.0f);
        p.velocity = Vec3(
            sin(phi) * cos(angle) * speed,
            sin(phi) * sin(angle) * speed,
            cos(phi) * speed
        );
        
        // Temperature spectrum colors: BRIGHT WHITE/GOLDEN for intense explosion
        float tempFactor = rand() % 100 / 100.0f;
        if(tempFactor < 0.4f) {
            // Bright white (50% of particles)
            p.color = Vec3(1.0f, 1.0f, 1.0f);
        } else if(tempFactor < 0.7f) {
            // Golden yellow
            p.color = Vec3(1.0f, 0.95f, 0.3f);
        } else if(tempFactor < 0.85f) {
            // Orange-gold
            p.color = Vec3(1.0f, 0.6f, 0.1f);
        } else {
            // Hot orange
            p.color = Vec3(1.0f, 0.4f, 0.0f);
        }
        
        p.alpha = 1.0f;  // Start full bright
        p.lifetime = 0.0f;
        p.type = 0;  // Plasma
        
        particles.push_back(p);
    }
}

void updateBigBangScene() {
    float bangTime = globalTime - 0.167f;
    
    for (size_t i = 0; i < particles.size(); ++i) {
        auto& p = particles[i];
        if (bangTime < 0.0f) return;  // Before Big Bang
        
        p.lifetime = bangTime;
        
        // EXPLOSIVE LAUNCH PHASE (first 0.5 seconds)
        if (bangTime < 0.5f) {
            // Particles maintain high energy, minimal deceleration
            float expFactor = 1.0f - bangTime * 0.1f;
            p.velocity *= (0.95f + expFactor * 0.05f);
        } else if (bangTime < 1.5f) {
            // Deceleration force field (gravity pulling inward)
            Vec3 centerDir = p.position * (-1.0f);
            float distSq = p.position.dot(p.position);
            if (distSq > 0.01f) {
                Vec3 centerForce = centerDir.normalize() * 0.08f * deltaTime;
                p.velocity += centerForce;
            }
            
            // Gradual slowdown with turbulence
            float slowdown = 1.0f - bangTime * 0.1f;
            if (slowdown < 0.2f) slowdown = 0.2f;
            p.velocity *= slowdown;
        } else if (bangTime < 5.0f) {
            // Star coalescence phase - slow approach to clusters
            int clusterIdx = (int)((i + 1) % 3000);
            Vec3 clusterPos = Vec3(
                sinf(clusterIdx * 0.005f) * 3.5f,
                cosf(clusterIdx * 0.008f) * 3.5f,
                sinf(clusterIdx * 0.006f) * 3.5f
            );
            
            Vec3 toCluster = clusterPos - p.position;
            float dist = toCluster.length();
            if (dist > 0.2f) {
                Vec3 clusterForce = toCluster.normalize() * 0.3f * deltaTime;
                p.velocity += clusterForce;
            }
            p.velocity *= (1.0f - bangTime * 0.05f);
        } else if (bangTime < 12.0f) {
            // Stabilization phase - particles lock into position
            float stabilizeFactor = (bangTime - 5.0f) / 7.0f;
            p.velocity *= (1.0f - stabilizeFactor * 0.95f);
        } else if (bangTime < 18.0f) {
            // Settling phase - minimal motion (universe at rest)
            p.velocity *= 0.95f;
        } else {
            // Fade phase - just maintain position
            p.velocity *= 0.0f;
        }
        
        p.position += p.velocity * deltaTime;
        float ageRatio = std::min(bangTime / 5.0f, 1.0f);
        if (bangTime < 0.5f) {
            // Keep bright explosion colors during launch
            p.alpha = 0.9f - ageRatio * 0.2f;
        } else if (bangTime < 3.0f) {
            // Cooling phase - colors shift toward orange/red
            float cool = (bangTime - 0.5f) / 2.5f;
            p.color = Vec3(p.color.x * (1.0f - cool * 0.3f), 
                          p.color.y * (1.0f - cool * 0.5f),
                          p.color.z * (1.0f - cool * 0.8f));
            p.alpha = 0.7f - cool * 0.3f;
        } else if (bangTime < 12.0f) {
            // Stabilization phase - colors deepen to dark red
            p.color = Vec3(0.8f, 0.2f, 0.1f);
            p.alpha = std::max(0.3f, 0.4f * (1.0f - (bangTime - 3.0f) / 9.0f));
        } else if (bangTime < 18.0f) {
            // Settling phase - colors maintain but gradually dim
            p.color = Vec3(0.6f, 0.15f, 0.08f);
            float settleAlpha = 0.3f * (1.0f - (bangTime - 12.0f) / 6.0f);
            p.alpha = std::max(0.1f, settleAlpha);
        } else {
            // Final fade to black (18-25s)
            float fadeProgress = (bangTime - 18.0f) / 7.0f;
            p.alpha = 0.3f * (1.0f - fadeProgress);
        }
        
        if (p.alpha < 0.0f) p.alpha = 0.0f;
    }
}

void drawBigBangScene() {
    float bangTime = globalTime - 0.167f;
    
    if (bangTime < 0.0f) {
        // Before Big Bang - black screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        return;
    }
    
    // Keep background BLACK throughout
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // MASSIVE SHOCKWAVE effect (3.0 seconds - extended for more impact!)
    if (bangTime < 3.0f) {
        float progress = bangTime / 3.0f;
        
        // Radial gradient shockwave with multiple layers
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        // SUPERCHARGED CORE - ULTRA BRIGHT white explosion at center
        float coreSize = 0.2f + progress * 0.15f;  // Larger core
        glPointSize(coreSize * 300.0f);  // MUCH BIGGER point
        glColor4f(1.0f, 1.0f, 0.95f, 1.0f);  // Almost fully opaque white-gold
        glBegin(GL_POINTS);
        glVertex2f(0.0f, 0.0f);
        glEnd();
        
        // ADDITIONAL SUPER-BRIGHT CORE LAYERS for ultra glow
        for (int coreLayer = 0; coreLayer < 3; ++coreLayer) {
            float layerSize = coreSize * (0.7f - coreLayer * 0.2f);
            float layerAlpha = 0.4f / (coreLayer + 1);
            glPointSize(layerSize * 250.0f);
            glColor4f(1.0f, 1.0f, 0.85f, layerAlpha);
            glBegin(GL_POINTS);
            glVertex2f(0.0f, 0.0f);
            glEnd();
        }
        
        // MASSIVE HORIZONTAL BEAM/DISK EFFECT (like reference image)
        if (progress < 1.0f) {
            // Draw horizontal expanding disk
            float diskRadius = progress * 2.2f;
            float diskThickness = 0.15f + progress * 0.2f;
            
            // Central bright beam along X-Y plane
            glLineWidth(5.0f);
            glColor4f(1.0f, 1.0f, 0.8f, (1.0f - progress) * 0.8f);
            
            // Horizontal expanding disk with radiating spikes
            int diskSegments = 24;
            for (int seg = 0; seg < diskSegments; ++seg) {
                float angle = (seg / (float)diskSegments) * 2.0f * 3.14159f;
                float x1 = cos(angle) * diskRadius * 0.3f;
                float y1 = sin(angle) * diskRadius * 0.3f;
                float x2 = cos(angle) * diskRadius;
                float y2 = sin(angle) * diskRadius;
                glBegin(GL_LINES);
                glVertex2f(x1, y1);
                glVertex2f(x2, y2);
                glEnd();
            }
            glLineWidth(1.0f);
        }
        
        // MULTIPLE EXPANDING RINGS - each with color gradient
        int totalRings = 120;  // INCREASED from 80 for more density
        for (int i = 0; i < totalRings; ++i) {
            float ringProgress = progress * (i / (float)totalRings);
            float ringAlpha = (1.0f - ringProgress) * 0.8f;
            
            // Color gradient: white → yellow → orange → red → deep red
            float r, g, b, a;
            if (ringProgress < 0.15f) {
                // White core
                float t = ringProgress / 0.15f;
                r = 1.0f;
                g = 1.0f;
                b = 1.0f;
                a = ringAlpha * (1.0f - t * 0.3f);
            } else if (ringProgress < 0.3f) {
                // White to yellow
                float t = (ringProgress - 0.15f) / 0.15f;
                r = 1.0f;
                g = 1.0f;
                b = 1.0f * (1.0f - t);
                a = ringAlpha;
            } else if (ringProgress < 0.5f) {
                // Yellow to orange
                float t = (ringProgress - 0.3f) / 0.2f;
                r = 1.0f;
                g = 0.8f * (1.0f - t * 0.5f);
                b = 0.0f;
                a = ringAlpha;
            } else if (ringProgress < 0.75f) {
                // Orange to red
                float t = (ringProgress - 0.5f) / 0.25f;
                r = 1.0f;
                g = 0.4f * (1.0f - t);
                b = 0.0f;
                a = ringAlpha * (1.0f - t * 0.3f);
            } else {
                // Deep red to transparent
                float t = (ringProgress - 0.75f) / 0.25f;
                r = 0.7f * (1.0f - t * 0.5f);
                g = 0.0f;
                b = 0.0f;
                a = ringAlpha * (1.0f - t) * 0.5f;
            }
            
            glColor4f(r, g, b, std::max(0.0f, a));
            
            // Draw expanding ring
            float radius = progress * 1.8f;
            int segments = 128;
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < segments; ++j) {
                float angle = j * 2.0f * 3.14159f / segments;
                // Add slight wobble for turbulence effect
                float wobble = sin(angle * 5.0f + globalTime * 10.0f) * 0.02f;
                glVertex2f((cos(angle) * (radius + wobble)), 
                          (sin(angle) * (radius + wobble)));
            }
            glEnd();
        }
        
        // SHOCK WAVE BLAST RADIAL BANDS - extra visual depth
        glLineWidth(3.0f);
        int blastBands = 24;  // DOUBLED from 12
        for (int band = 0; band < blastBands; ++band) {
            float bandProgress = progress * 0.8f;
            float bandAlpha = (1.0f - bandProgress) * 0.6f;
            
            float hue = band / (float)blastBands;
            float r = sin(hue * 3.14159f);
            float g = cos(hue * 3.14159f + 1.0f);
            float b = sin(hue * 3.14159f + 2.0f);
            
            glColor4f(r * 0.7f + 0.3f, g * 0.7f + 0.3f, b * 0.7f + 0.3f, bandAlpha);
            
            float blastRadius = bandProgress * 1.5f;
            int segments = 32;
            
            // Radial lines from center outward (starburst effect)
            for (int i = 0; i < segments; ++i) {
                float angle = (i / (float)segments) * 2.0f * 3.14159f;
                glBegin(GL_LINES);
                glVertex2f(0.0f, 0.0f);
                glVertex2f(cos(angle) * blastRadius, sin(angle) * blastRadius);
                glEnd();
            }
        }
        glLineWidth(1.0f);
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
    }
    
    // Draw plasma particles with EXTREME brightness during burst
    glPointSize(8.0f);  // LARGER particles
    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        // MUCH BRIGHTER particles during explosion
        float brightness = 1.2f;  // Base brightness increased
        if (bangTime < 2.0f) {
            brightness = 1.5f + sin(bangTime * 8.0f) * 0.5f;  // INTENSE pulsing
        } else if (bangTime < 3.0f) {
            brightness = 1.3f + sin(bangTime * 5.0f) * 0.3f;
        }
        
        glColor4f(p.color.x * brightness, p.color.y * brightness, p.color.z * brightness, p.alpha);
        glVertex3f(p.position.x, p.position.y, p.position.z);
    }
    glEnd();
    
    // Draw MULTIPLE HALO LAYERS for extreme glow effect
    // Layer 1: Medium halos
    glPointSize(18.0f);
    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        float hazeAlpha = p.alpha * 0.25f;  // Increased from 0.15f
        glColor4f(p.color.x * 0.9f, p.color.y * 0.9f, p.color.z * 0.9f, hazeAlpha);
        glVertex3f(p.position.x, p.position.y, p.position.z);
    }
    glEnd();
    
    // Layer 2: Large atmospheric glow
    glPointSize(30.0f);
    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        float hazeAlpha = p.alpha * 0.12f;
        glColor4f(p.color.x * 0.7f, p.color.y * 0.7f, p.color.z * 0.7f, hazeAlpha);
        glVertex3f(p.position.x, p.position.y, p.position.z);
    }
    glEnd();
    
    // EXTENDED ANIMATION PHASES (beyond 5 seconds)
    // Phase 1: Particle stabilization and cluster formation (5-12 seconds)
    if (bangTime > 5.0f && bangTime < 12.0f) {
        float phaseTime = bangTime - 5.0f;
        
        // Draw connecting lines between nearby particles (forming structures)
        if (phaseTime > 2.0f) {
            float clusterAlpha = (phaseTime - 2.0f) / 5.0f;
            glColor4f(0.2f, 0.6f, 1.0f, clusterAlpha * 0.3f);
            glLineWidth(0.5f);
            glBegin(GL_LINES);
            
            // Draw sparse connecting lines between particles
            for (size_t i = 0; i < particles.size(); i += 500) {
                for (size_t j = i + 1; j < particles.size() && j < i + 1000; j += 100) {
                    float dist = (particles[i].position - particles[j].position).length();
                    if (dist < 1.5f && dist > 0.1f) {
                        glVertex3f(particles[i].position.x, particles[i].position.y, particles[i].position.z);
                        glVertex3f(particles[j].position.x, particles[j].position.y, particles[j].position.z);
                    }
                }
            }
            glEnd();
            glLineWidth(1.0f);
        }
    }
    
    // Phase 2: Quiet universe settling (12-18 seconds)
    if (bangTime > 12.0f && bangTime < 18.0f) {
        float settleAlpha = (bangTime - 12.0f) / 6.0f;
        
        // Dim starfield background effect
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        // Subtle background stars scattered
        glPointSize(0.5f);
        glColor4f(1.0f, 1.0f, 1.0f, settleAlpha * 0.15f);
        glBegin(GL_POINTS);
        for (int i = 0; i < 200; ++i) {
            float randX = -1.0f + (rand() % 2000) / 1000.0f;
            float randY = -1.0f + (rand() % 2000) / 1000.0f;
            glVertex2f(randX, randY);
        }
        glEnd();
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
    }
    
    // Phase 3: Universe stillness and fade (18-25 seconds)
    if (bangTime > 18.0f) {
        float fadeTime = bangTime - 18.0f;
        
        // Gentle pulsing of remaining particles
        for (auto& p : particles) {
            if (p.alpha > 0.01f) {
                p.alpha *= (0.98f + sin(bangTime * 1.5f) * 0.02f);
            }
        }
        
        // Screen fade to black
        if (fadeTime > 5.0f) {
            glDisable(GL_DEPTH_TEST);
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            
            float blackAlpha = (fadeTime - 5.0f) / 2.0f;
            if (blackAlpha > 1.0f) blackAlpha = 1.0f;
            
            glColor4f(0.0f, 0.0f, 0.0f, blackAlpha);
            glBegin(GL_QUADS);
            glVertex2f(-1.0f, -1.0f);
            glVertex2f(1.0f, -1.0f);
            glVertex2f(1.0f, 1.0f);
            glVertex2f(-1.0f, 1.0f);
            glEnd();
            
            glPopMatrix();
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glEnable(GL_DEPTH_TEST);
        }
    }
}

// ============================================================================
// SCENE MANAGEMENT
// ============================================================================

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Initialize shader functions
    initShaderFunctions();
    
    // Shaders are optional - animation works without them
    if (glCreateShader) {
        std::cout << "Shader support available." << std::endl;
    } else {
        std::cout << "Shaders not available - using particle-based rendering." << std::endl;
    }
    
    // Initialize scene
    initBigBangScene();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
    
    updateBigBangScene();
    drawBigBangScene();
    
    glutSwapBuffers();
}

void idle() {
    globalTime += deltaTime * TIME_SCALE;
    
    // Hold final frame after story timeline completes (real runtime ~= 5 minutes).
    if (globalTime > STORY_DURATION) {
        std::cout << "Chapter 2 complete. Big Bang evolution finished." << std::endl;
        globalTime = STORY_DURATION;
    }
    
    glutPostRedisplay();
}

void timer(int value) {
    idle();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC
        exit(0);
    } else if (key == '1') {
        currentScene = 1;
        globalTime = 0.0f;
    } else if (key == '2') {
        currentScene = 2;
        globalTime = 0.0f;
    }
}

// ============================================================================
// MAIN
// ============================================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Cosmos - Big Bang Evolution");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    init();
    
    glutTimerFunc(16, timer, 0);
    
    std::cout << "===== COSMOS: Big Bang Evolution =====" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "  1   - Chaos scene" << std::endl;
    std::cout << "  2   - Big Bang scene" << std::endl;
    std::cout << "\nAnimation Timeline:" << std::endl;
    std::cout << "  t=0.167s - THE BIG BANG" << std::endl;
    std::cout << "  t=0.167s-1.667s - Shockwave expands" << std::endl;
    std::cout << "  t=2.0s+ - Stars coalesce from plasma" << std::endl;
    std::cout << "  t=8.0s+ - Background brightens to space blue" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    glutMainLoop();
    return 0;
}
