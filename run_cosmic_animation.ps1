# Cosmic Dance - 5 Minute Animation Sequence
# Plays all 17 chapters sequentially with automatic transitions

# Chapters configuration with durations
# Executables match Makefile output names
$chapters = @(
    @{name='chapter1_chaos'; exe='chapter1_chaos.exe'; title='Chaos and Darkness'; duration=18},
    @{name='chapter2_bigbang'; exe='chapter2_bigbang.exe'; title='The Big Bang'; duration=18},
    @{name='nebula_genesis'; exe='nebula_genesis.exe'; title='Stellar Nurseries - Nebula Genesis'; duration=18},
    @{name='cosmic_observer'; exe='cosmic_observer.exe'; title='The Cosmic Observer'; duration=18},
    @{name='galaxy_milkyway'; exe='galaxy_milkyway.exe'; title='The Milky Way Galaxy'; duration=18},
    @{name='chapter3_accurate_solar'; exe='chapter3_accurate_solar.exe'; title='Formation of the Solar System'; duration=18},
    @{name='emergence_of_life'; exe='emergence_of_life.exe'; title='Emergence of Life'; duration=18},
    @{name='first_cells'; exe='first_cells.exe'; title='The First Cells'; duration=18},
    @{name='multicellular_organism'; exe='multicellular_organism.exe'; title='Multicellular Life'; duration=18},
    @{name='chapter10_early_organism'; exe='chapter10_early_organism.exe'; title='Cambrian Explosion of Life'; duration=18},
    @{name='chapter11_complex_organism'; exe='chapter11_complex_organism.exe'; title='Ordovician-Silurian - Complex Marine Life'; duration=18},
    @{name='chapter12_ocean_to_land'; exe='chapter12_ocean_to_land.exe'; title='Devonian - Life Moves to Land'; duration=18},
    @{name='chapter13_land_volcanos'; exe='chapter13_land_volcanos.exe'; title='Carboniferous - Forests and Amphibians'; duration=18},
    @{name='chapter14_dinosaur_era'; exe='chapter14_dinosaur_era.exe'; title='Jurassic - Age of Dinosaurs'; duration=18},
    @{name='chapter15_mammalian_rise'; exe='chapter15_mammalian_rise.exe'; title='Cretaceous - Rise of Mammals'; duration=18},
    @{name='chapter17_humanity_observes'; exe='chapter17_humanity_observes.exe'; title='Humanity Observes the Universe (FINALE)'; duration=30}
)

# Display header
Clear-Host
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "COSMIC DANCE - 5 MINUTE ANIMATION" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Sequential playback of all 17 chapters" -ForegroundColor Green
Write-Host "Total Duration: ~5 minutes" -ForegroundColor Green
Write-Host ""
Write-Host "Press Ctrl+C to exit at any time" -ForegroundColor Yellow
Write-Host ""

# Compile all chapters first
Write-Host "Compiling chapters..." -ForegroundColor Yellow
$buildResult = mingw32-make clean 2>&1 | Out-Null
$buildResult = mingw32-make all 2>&1 | Out-Null

if ($LASTEXITCODE -ne 0) {
    Write-Host "Error compiling chapters!" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host "Compilation complete! Starting animation..." -ForegroundColor Green
Write-Host ""

$totalDuration = 0
$currentChapter = 1

# Play each chapter
foreach ($chapter in $chapters) {
    Write-Host "[${currentChapter}/16] $($chapter.title)" -ForegroundColor Magenta
    Write-Host "Duration: $($chapter.duration) seconds" -ForegroundColor Gray
    
    # Check if executable exists
    if (-not (Test-Path $chapter.exe)) {
        Write-Host "ERROR: $($chapter.exe) not found!" -ForegroundColor Red
        $currentChapter++
        continue
    }
    
    # Start the process
    $process = Start-Process $chapter.exe -PassThru
    
    # Wait for the specified duration
    Start-Sleep -Seconds $chapter.duration
    
    # Kill the process
    try {
        Stop-Process -InputObject $process -Force -ErrorAction SilentlyContinue
    } catch {
        # Process already exited, that's fine
    }
    
    $totalDuration += $chapter.duration
    $currentChapter++
    Write-Host ""
}

# Show completion screen
Clear-Host
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "COSMIC DANCE - COMPLETE" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "You have witnessed the cosmic journey:" -ForegroundColor Green
Write-Host ""
Write-Host "Stage 1: Cosmic Origins" -ForegroundColor Magenta
Write-Host "  Chapter 1:  Chaos and Darkness" -ForegroundColor Gray
Write-Host "  Chapter 2:  The Big Bang" -ForegroundColor Gray
Write-Host "  Chapter 3:  Stellar Nurseries" -ForegroundColor Gray
Write-Host "  Chapter 4:  The Cosmic Observer" -ForegroundColor Gray
Write-Host "  Chapter 5:  The Milky Way Galaxy" -ForegroundColor Gray
Write-Host "  Chapter 6:  Formation of the Solar System" -ForegroundColor Gray
Write-Host ""
Write-Host "Stage 2: Life Emerges" -ForegroundColor Magenta
Write-Host "  Chapter 7:  Emergence of Life" -ForegroundColor Gray
Write-Host "  Chapter 8:  The First Cells" -ForegroundColor Gray
Write-Host "  Chapter 9:  Multicellular Life" -ForegroundColor Gray
Write-Host "  Chapter 10: Cambrian Explosion" -ForegroundColor Gray
Write-Host "  Chapter 11: Complex Marine Life" -ForegroundColor Gray
Write-Host ""
Write-Host "Stage 3: Life on Land" -ForegroundColor Magenta
Write-Host "  Chapter 12: Life Moves to Land" -ForegroundColor Gray
Write-Host "  Chapter 13: Forests and Amphibians" -ForegroundColor Gray
Write-Host "  Chapter 14: Age of Dinosaurs" -ForegroundColor Gray
Write-Host "  Chapter 15: Rise of Mammals" -ForegroundColor Gray
Write-Host ""
Write-Host "Stage 4: Consciousness Awakens" -ForegroundColor Magenta
Write-Host "  Chapter 17: Humanity Observes" -ForegroundColor Gray
Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Total Runtime: Approximately 5 minutes" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

Read-Host "Press Enter to exit"
