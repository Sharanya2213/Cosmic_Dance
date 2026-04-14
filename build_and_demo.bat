@echo off
REM Cosmic Dance - Build and Demo Script (Windows Batch)
REM Complete build automation for all 16 chapters

setlocal enabledelayedexpansion

title Cosmic Dance - Build System
color 0F

echo.
echo ==========================================
echo COSMIC DANCE - 16-Chapter Animation Series
echo ==========================================
echo.

REM Check if mingw32-make exists
where mingw32-make >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    color CF
    echo ERROR: mingw32-make not found. Please install MinGW.
    pause
    exit /b 1
)

:menu
cls
echo ==========================================
echo COSMIC DANCE - Build and Run Menu
echo ==========================================
echo.
echo Choose an action:
echo   1. Build all chapters
echo   2. Build specific chapter
echo   3. Run specific chapter
echo   4. Clean all builds
echo   5. Show chapter descriptions
echo   6. Verify all executables
echo   7. Build and run quick demo
echo   0. Exit
echo.
set /p choice="Enter choice [0-7]: "

if "%choice%"=="1" goto build_all
if "%choice%"=="2" goto build_specific
if "%choice%"=="3" goto run_specific
if "%choice%"=="4" goto clean_all
if "%choice%"=="5" goto show_descriptions
if "%choice%"=="6" goto verify_executables
if "%choice%"=="7" goto quick_demo
if "%choice%"=="0" goto exit_script
echo Invalid choice. Please try again.
pause
goto menu

:build_all
echo.
echo Building all chapters...
mingw32-make all
if %ERRORLEVEL% EQU 0 (
    color 2F
    echo.
    echo All chapters built successfully!
    color 0F
) else (
    color CF
    echo Build failed. Check errors above.
    color 0F
)
pause
goto menu

:build_specific
echo.
set /p chapter="Enter chapter number (1-16): "
if %chapter% LSS 1 goto invalid_chapter
if %chapter% GTR 16 goto invalid_chapter
echo Building Chapter %chapter%...
mingw32-make chapter%chapter%*.exe
pause
goto menu

:invalid_chapter
color CF
echo Invalid chapter number. Must be 1-16.
color 0F
pause
goto menu

:run_specific
echo.
set /p chapter="Enter chapter number to run (1-16): "
if %chapter% LSS 1 goto invalid_chapter
if %chapter% GTR 16 goto invalid_chapter
echo Running Chapter %chapter%...
mingw32-make run%chapter%
pause
goto menu

:clean_all
echo.
echo Cleaning all builds...
mingw32-make clean
echo Clean complete.
pause
goto menu

:show_descriptions
cls
echo ==========================================
echo COSMIC DANCE - 16 CHAPTERS
echo ==========================================
echo.
echo PART 1: COSMIC ORIGINS
echo   Ch 1:  Chaos and Darkness - Primordial universe
echo   Ch 2:  The Big Bang - Birth of universe
echo   Ch 3:  Accurate Solar System - 8 planets with real orbits
echo   Ch 4:  Nebula Genesis - Star birth from dust
echo   Ch 5:  Milky Way Galaxy - Galactic structure
echo   Ch 6:  Cosmic Observer - Zooming through space
echo.
echo PART 2: LIFE EMERGES
echo   Ch 7:  Emergence of Life - Prebiotic chemistry
echo   Ch 8:  First Cells - Single-celled organisms
echo   Ch 9:  Multicellular Organism - Cells clustering
echo.
echo PART 3: ORGANISM EVOLUTION
echo   Ch 10: Early Organism Movement - Swimming in oceans
echo   Ch 11: Complex Organism - First fish with fins
echo   Ch 12: Ocean to Land - Fish-tetrapod transition
echo.
echo PART 4: TERRESTRIAL LIFE
echo   Ch 13: Land with Volcanoes & Gymnosperms - Ancient environments
echo   Ch 14: Age of Dinosaurs - T-Rex, Triceratops, Stegosaurus
echo   Ch 15: Asteroid Impact - Extinction event
echo   Ch 16: Mammalian Rise - Post-extinction mammals
echo.
pause
goto menu

:verify_executables
echo.
echo Verifying executables...
setlocal enabledelayedexpansion
set count=0
for /L %%i in (1,1,16) do (
    if exist chapter%%i*.exe (
        echo   ✓ Chapter %%i executable found
        set /a count+=1
    ) else (
        echo   ✗ Chapter %%i NOT found
    )
)
echo.
echo Total executables found: !count!/16
echo.
pause
goto menu

:quick_demo
echo.
echo Building and running Chapter 1: Chaos and Darkness...
mingw32-make chapter1_chaos_darkness.exe
if %ERRORLEVEL% EQU 0 (
    echo Running demo...
    mingw32-make run1
) else (
    echo Build failed.
)
pause
goto menu

:exit_script
echo.
echo Exiting Cosmic Dance. Goodbye!
echo.
exit /b 0
