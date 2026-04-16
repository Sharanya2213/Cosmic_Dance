@echo off
REM Cosmic Dance - 5 Minute Animation Sequence
REM Plays all 17 chapters sequentially with fade transitions
REM Each chapter: ~18 seconds (5 minutes / 17 chapters)

echo.
echo ========================================
echo COSMIC DANCE - 5 MINUTE ANIMATION
echo ========================================
echo Starting 5-minute cosmic journey...
echo Press Ctrl+C to exit at any time
echo.

REM Colors and headers for each chapter
set CHAPTER_DURATION=18

REM Chapter 1 - Chaos and Darkness
echo [1/17] Chapter 1: Chaos and Darkness (%CHAPTER_DURATION%s)
start /wait chapter1_chaos_darkness.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 2 - The Big Bang
echo [2/17] Chapter 2: The Big Bang (%CHAPTER_DURATION%s)
start /wait chapter2_bigbang.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 3 - Stellar Nurseries
echo [3/17] Chapter 3: Stellar Nurseries - Nebula Genesis (%CHAPTER_DURATION%s)
start /wait chapter3_nebula_genesis.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 4 - The Cosmic Observer
echo [4/17] Chapter 4: The Cosmic Observer (%CHAPTER_DURATION%s)
start /wait chapter4_cosmic_observer.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 5 - The Milky Way Galaxy
echo [5/17] Chapter 5: The Milky Way Galaxy (%CHAPTER_DURATION%s)
start /wait chapter5_galaxy_milkyway.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 6 - Formation of the Solar System
echo [6/17] Chapter 6: Formation of the Solar System (%CHAPTER_DURATION%s)
start /wait chapter6_accurate_solar.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 7 - Precambrian Era - Emergence of Life
echo [7/17] Chapter 7: Precambrian Era - Emergence of Life (%CHAPTER_DURATION%s)
start /wait chapter7_emergence_of_life.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 8 - Precambrian - The First Cells
echo [8/17] Chapter 8: Precambrian - The First Cells (%CHAPTER_DURATION%s)
start /wait chapter8_first_cells.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 9 - Precambrian - Multicellular Life
echo [9/17] Chapter 9: Precambrian - Multicellular Life (%CHAPTER_DURATION%s)
start /wait chapter9_multicellular_organism.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 10 - Cambrian Period - Explosion of Life
echo [10/17] Chapter 10: Cambrian Period - Explosion of Life (%CHAPTER_DURATION%s)
start /wait chapter10_early_organism.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 11 - Ordovician-Silurian - Complex Marine Life
echo [11/17] Chapter 11: Ordovician-Silurian - Complex Marine Life (%CHAPTER_DURATION%s)
start /wait chapter11_complex_organism.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 12 - Devonian Period - Life Moves to Land
echo [12/17] Chapter 12: Devonian Period - Life Moves to Land (%CHAPTER_DURATION%s)
start /wait chapter12_ocean_to_land.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 13 - Carboniferous Period - Forests and Amphibians
echo [13/17] Chapter 13: Carboniferous Period - Forests and Amphibians (%CHAPTER_DURATION%s)
start /wait chapter13_land_volcanos.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 14 - Jurassic Period - Age of Dinosaurs (Chapter 16 in display)
echo [14/17] Chapter 14: Jurassic Period - Age of Dinosaurs (%CHAPTER_DURATION%s)
start /wait chapter14_dinosaur_era.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 15 - Cretaceous Period - Rise of Mammals
echo [15/17] Chapter 15: Cretaceous Period - Rise of Mammals (%CHAPTER_DURATION%s)
start /wait chapter15_mammalian_rise.exe
timeout /t %CHAPTER_DURATION% /nobreak

REM Chapter 17 - Humanity Observes the Universe (Extended time)
echo [16/17] Chapter 17: Humanity Observes the Universe (30s)
start /wait chapter17_humanity_observes.exe
timeout /t 30 /nobreak

REM End credits
echo.
echo ========================================
echo Animation Complete!
echo Total Duration: ~5 minutes
echo Thank you for watching Cosmic Dance
echo ========================================
echo.
pause
