# Chaos and Darkness - 3D Animation

A stunning 3D animation showcasing chaos and darkness using C++, FreeGLUT, and OpenGL.

## Features

- **Breathing Background**: Dark background that slowly changes color using sine wave animation (8-second period)
- **Particle System**: ~2000 particles with swirling motion, gradually increasing speeds
- **Lightning Effects**: Random lightning strikes every 6-10 seconds with color transitions and flickering
- **Center Light**: Glowing point that appears after 40 seconds and slowly expands with flickering
- **Title Text**: "CHAOS AND DARKNESS" displayed with fade-in and fade-out effects
- **Blending Effects**: GL_BLEND with GL_SRC_ALPHA and GL_ONE for realistic glow effects
- **Smooth Animation**: 60 FPS animation using glutIdleFunc and glutTimerFunc

## System Requirements

- Windows OS (for MinGW compilation)
- MinGW C++ compiler (g++)
- FreeGLUT library
- OpenGL drivers

## Installation

### 1. Install MinGW and FreeGLUT

If you don't have MinGW installed:
- Download from: https://www.mingw-w64.org/
- Install with desired configuration

For FreeGLUT:
- Download pre-built libraries from: http://freeglut.sourceforge.net/
- Or use package manager if available

### 2. Setup FreeGLUT Libraries

Make sure you have:
- `freeglut.lib` or `libfreeglut.a` (static library)
- `freeglut.dll` (runtime)
- Header files in `GL/` directory

Common MinGW location:
```
C:\MinGW\lib\        (libraries)
C:\MinGW\include\GL\ (headers)
```

## Compilation

### Using Makefile (Recommended)

```bash
mingw32-make all
```

Or just:
```bash
mingw32-make
```

### Using Command Line (Alternative)

```bash
g++ -std=c++11 -O2 -Wall chaos_darkness.cpp -o chaos_darkness.exe -lopengl32 -lglu32 -lfreeglut
```

### Troubleshooting Compilation

**Error**: "cannot find -lfreeglut"
- Solution: Ensure FreeGLUT is installed and library path is correct
- Or link specific path: `g++ ... -L"C:\path\to\MinGW\lib" -lfreeglut ...`

**Error**: "GL/glut.h: No such file or directory"
- Solution: Install FreeGLUT headers or set include path:
- `g++ -I"C:\path\to\MinGW\include" ...`

## Running

After successful compilation:

```bash
mingw32-make run
```

Or directly:
```bash
./chaos_darkness.exe
```

## Controls

- **ESC**: Exit the application
- **Window**: Resizable - animation will adjust to window size

## Animation Timeline

- **0-4 seconds**: Title text appears with fade-in/fade-out
- **Throughout**: Breathing background, particle swirls, random lightning strikes
- **40+ seconds**: Glowing center light appears and slowly expands
- **Continuous**: All effects run indefinitely with smooth 60 FPS animation

## Code Features

### Technical Details

- **Window Setup**: 1200x800, double-buffered, RGBA color
- **Particle Count**: 2000 particles for good visual density without performance loss
- **Color Scheme**: Dark purples, reds, and indigo with varying alpha
- **Blending**: GL_BLEND with GL_SRC_ALPHA and GL_ONE for additive glow
- **Depth Testing**: Enabled for proper 3D layering
- **Camera**: Static orthographic view at (0, 0.5, 3) looking at origin
- **No Shaders**: Uses classic OpenGL (glBegin/glEnd) for compatibility

### Key Components

1. **Particle System**
   - Swirling motion using sin/cos approximation of curl noise
   - Velocity-based movement with wrapping boundaries
   - Dark purple to red color palette

2. **Lightning**
   - Random generation every 6-10 seconds
   - Jagged lines with jittering segments
   - Color transition: white → purple → fade
   - Flicker effect for realism

3. **Center Light**
   - Appears after 40 seconds
   - Slowly grows over 15 seconds
   - Flickering effect using high-frequency sine wave
   - Dual layer (bright point + dim halo)

4. **Background**
   - Smooth color breathing using 8-second sine period
   - Black to very dark indigo transitions

## Performance

- Target: 60 FPS
- Typical performance: Smooth on modern hardware
- Memory: Minimal (~2MB including particles)
- GPU: Any modern GPU sufficient

## Notes

- Animation is fully time-based, allowing for smooth pausing/resuming if needed
- Particles wrap around space to create continuous motion
- All effects use global time variable for synchronization
- Code is organized for easy modification of parameters

## Customization

You can easily modify:
- `WINDOW_WIDTH`/`WINDOW_HEIGHT`: Window resolution
- `NUM_PARTICLES`: Number of particles (2000 recommended)
- Colors in `initializeParticles()`: Particle colors
- Lightning timing: `6.0f` and `4000` values in `updateLightning()`
- Camera position: `cameraX`, `cameraY`, `cameraZ`
- Animation speeds: Various multipliers in update functions

## Author Notes

This is a demonstration of visual effects using classic OpenGL and FreeGLUT. The "chaos" is represented through the swirling particles and random lightning, while "darkness" is conveyed through the dark color palette and low alpha values. The center light emerging after 40 seconds symbolizes light breaking through chaos.

Enjoy the animation!
