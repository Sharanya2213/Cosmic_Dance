@echo off
REM Cosmic Dance - 5 Minute Animation Sequence
REM Plays all 17 chapters sequentially with fade transitions
REM Each chapter: ~18 seconds (5 minutes / 17 chapters)

echo.
echo ========================================
echo COSMIC DANCE - 5 MINUTE ANIMATION
echo ========================================
echo Starting 5-minute cosmic journey...
echo Each chapter runs for approximately 18 seconds
echo Press Ctrl+C in any window to exit
echo.

setlocal enabledelayedexpansion

REM Each chapter gets ~18 seconds (5 min / 17 chapters)
set CHAPTER_DURATION=18

REM Build all chapters first
echo Compiling all chapters...
call mingw32-make clean > nul 2>&1
call mingw32-make all > nul 2>&1

if %ERRORLEVEL% NEQ 0 (
    echo Error compiling chapters. Please check your build.
    pause
    exit /b 1
)

echo Compilation complete. Starting animation...
echo.

REM Chapter 1 - Chaos and Darkness
echo [1/17] Chapter 1: Chaos and Darkness - %CHAPTER_DURATION% seconds
start /b chapter1_chaos.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM chapter1_chaos.exe /F > nul 2>&1

REM Chapter 2 - The Big Bang
echo [2/17] Chapter 2: The Big Bang - %CHAPTER_DURATION% seconds
start /b chapter2_bigbang.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM chapter2_bigbang.exe /F > nul 2>&1

REM Chapter 3 - Stellar Nurseries - Nebula Genesis
echo [3/17] Chapter 3: Stellar Nurseries - Nebula Genesis - %CHAPTER_DURATION% seconds
start /b nebula_genesis.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM nebula_genesis.exe /F > nul 2>&1

REM Chapter 4 - The Cosmic Observer
echo [4/17] Chapter 4: The Cosmic Observer - %CHAPTER_DURATION% seconds
start /b cosmic_observer.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM cosmic_observer.exe /F > nul 2>&1

REM Chapter 5 - The Milky Way Galaxy
echo [5/17] Chapter 5: The Milky Way Galaxy - %CHAPTER_DURATION% seconds
start /b galaxy_milkyway.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM galaxy_milkyway.exe /F > nul 2>&1

REM Chapter 6 - Formation of the Solar System
echo [6/17] Chapter 6: Formation of the Solar System - %CHAPTER_DURATION% seconds
start /b chapter3_accurate_solar.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM chapter3_accurate_solar.exe /F > nul 2>&1

REM Chapter 7 - Emergence of Life
echo [7/17] Chapter 7: Emergence of Life - %CHAPTER_DURATION% seconds
start /b emergence_of_life.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM emergence_of_life.exe /F > nul 2>&1

REM Chapter 8 - The First Cells
echo [8/17] Chapter 8: The First Cells - %CHAPTER_DURATION% seconds
start /b first_cells.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM first_cells.exe /F > nul 2>&1

REM Chapter 9 - Multicellular Organisms
echo [9/17] Chapter 9: Multicellular Organisms - %CHAPTER_DURATION% seconds
start /b multicellular_organism.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM multicellular_organism.exe /F > nul 2>&1

REM Chapter 10 - Early Organism (Cambrian Explosion)
echo [10/17] Chapter 10: Cambrian Explosion of Life - %CHAPTER_DURATION% seconds
start /b chapter10_early_organism.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM chapter10_early_organism.exe /F > nul 2>&1

REM Chapter 11 - Complex Organisms
echo [11/17] Chapter 11: Ordovician-Silurian - Complex Marine Life - %CHAPTER_DURATION% seconds
start /b chapter11_complex_organism.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM chapter11_complex_organism.exe /F > nul 2>&1

REM Chapter 12 - Ocean to Land
echo [12/17] Chapter 12: Devonian - Life Moves to Land - %CHAPTER_DURATION% seconds
start /b chapter12_ocean_to_land.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM chapter12_ocean_to_land.exe /F > nul 2>&1

REM Chapter 13 - Land and Volcanos
echo [13/17] Chapter 13: Carboniferous - Forests and Amphibians - %CHAPTER_DURATION% seconds
start /b chapter13_land_volcanos.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM chapter13_land_volcanos.exe /F > nul 2>&1

REM Chapter 14 - Dinosaur Era
echo [14/17] Chapter 14: Jurassic - Age of Dinosaurs - %CHAPTER_DURATION% seconds
start /b chapter14_dinosaur_era.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM chapter14_dinosaur_era.exe /F > nul 2>&1

REM Chapter 15 - Mammalian Rise
echo [15/17] Chapter 15: Cretaceous - Rise of Mammals - %CHAPTER_DURATION% seconds
start /b chapter15_mammalian_rise.exe
timeout /t %CHAPTER_DURATION% /nobreak
taskkill /IM chapter15_mammalian_rise.exe /F > nul 2>&1

REM Chapter 17 - Humanity Observes (Extended finale)
echo [16/17] Chapter 17: Humanity Observes the Universe - 30 seconds (FINALE)
start /b chapter17_humanity_observes.exe
timeout /t 30 /nobreak
taskkill /IM chapter17_humanity_observes.exe /F > nul 2>&1

REM Final message
cls
echo.
echo ========================================
echo    COSMIC DANCE - COMPLETE
echo ========================================
echo.
echo You have witnessed the cosmic journey:
echo  - Chapter 1:  Chaos and Darkness
echo  - Chapter 2:  The Big Bang
echo  - Chapter 3:  Stellar Nurseries
echo  - Chapter 4:  The Cosmic Observer
echo  - Chapter 5:  The Milky Way Galaxy
echo  - Chapter 6:  Formation of the Solar System
echo  - Chapter 7:  Emergence of Life
echo  - Chapter 8:  The First Cells
echo  - Chapter 9:  Multicellular Life
echo  - Chapter 10: Cambrian Explosion
echo  - Chapter 11: Complex Marine Life
echo  - Chapter 12: Life Moves to Land
echo  - Chapter 13: Forests and Amphibians
echo  - Chapter 14: Age of Dinosaurs
echo  - Chapter 15: Rise of Mammals
echo  - Chapter 17: Humanity Observes
echo.
echo Total Runtime: Approximately 5 minutes
echo ========================================
echo.
pause
