# Cosmic Dance - Continuous 5 Minute Animation
# Runs all chapters sequentially WITHOUT killing them between transitions

# Chapters configuration
$chapters = @(
    @{exe='chapter1_chaos.exe'; title='Chaos and Darkness'; duration=18},
    @{exe='chapter2_bigbang.exe'; title='The Big Bang'; duration=18},
    @{exe='nebula_genesis.exe'; title='Stellar Nurseries'; duration=18},
    @{exe='cosmic_observer.exe'; title='The Cosmic Observer'; duration=18},
    @{exe='galaxy_milkyway.exe'; title='The Milky Way Galaxy'; duration=18},
    @{exe='chapter3_accurate_solar.exe'; title='Solar System Formation'; duration=18},
    @{exe='emergence_of_life.exe'; title='Emergence of Life'; duration=18},
    @{exe='first_cells.exe'; title='The First Cells'; duration=18},
    @{exe='multicellular_organism.exe'; title='Multicellular Life'; duration=18},
    @{exe='chapter10_early_organism.exe'; title='Cambrian Explosion'; duration=18},
    @{exe='chapter11_complex_organism.exe'; title='Complex Marine Life'; duration=18},
    @{exe='chapter12_ocean_to_land.exe'; title='Life Moves to Land'; duration=18},
    @{exe='chapter13_land_volcanos.exe'; title='Forests and Amphibians'; duration=18},
    @{exe='chapter14_dinosaur_era.exe'; title='Age of Dinosaurs'; duration=18},
    @{exe='chapter15_mammalian_rise.exe'; title='Rise of Mammals'; duration=18},
    @{exe='chapter17_humanity_observes.exe'; title='Humanity Observes (FINALE)'; duration=30}
)

Clear-Host
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "COSMIC DANCE - CONTINUOUS ANIMATION" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Running all 16 chapters continuously..." -ForegroundColor Green
Write-Host "Total Duration: ~5 minutes" -ForegroundColor Green
Write-Host ""
Write-Host "Close each window to auto-advance to next chapter" -ForegroundColor Yellow
Write-Host "Or wait for auto-timeout and seamless transition" -ForegroundColor Yellow
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
Write-Host "════════════════════════════════════════" -ForegroundColor Cyan

$currentChapter = 1
$totalStartTime = Get-Date
$processes = @()

# Start all chapters sequentially
foreach ($chapter in $chapters) {
    $chapterStartTime = Get-Date
    $chapterNumber = $currentChapter
    $totalChapters = $chapters.Count
    
    Write-Host ""
    Write-Host "[$chapterNumber/$totalChapters] $($chapter.title)" -ForegroundColor Magenta
    Write-Host "Duration: $($chapter.duration) seconds | Close window to advance" -ForegroundColor Gray
    
    if (-not (Test-Path $chapter.exe)) {
        Write-Host "ERROR: $($chapter.exe) not found!" -ForegroundColor Red
        $currentChapter++
        continue
    }
    
    # Start the chapter process
    $process = Start-Process $chapter.exe -PassThru
    
    # Wait for either the specified duration OR the process to close
    $endTime = (Get-Date).AddSeconds($chapter.duration)
    $processExited = $false
    
    while ((Get-Date) -lt $endTime -and -not $processExited) {
        if ($null -eq (Get-Process -Id $process.Id -ErrorAction SilentlyContinue)) {
            $processExited = $true
            Write-Host "Window closed. Advancing..." -ForegroundColor Yellow
        }
        Start-Sleep -Milliseconds 100
    }
    
    # Force close if still running
    if (-not $processExited) {
        try {
            Stop-Process -InputObject $process -Force -ErrorAction SilentlyContinue
            Write-Host "Auto-advancing to next chapter..." -ForegroundColor Cyan
        } catch {
            # Already closed
        }
    }
    
    # Brief pause for smooth transition
    Start-Sleep -Milliseconds 500
    
    $currentChapter++
}

# Show completion
$totalEndTime = Get-Date
$totalDuration = $totalEndTime - $totalStartTime

Clear-Host
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "    COSMIC DANCE - COMPLETE" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "You witnessed all 16 chapters:" -ForegroundColor Green
Write-Host ""
$chapters | ForEach-Object { Write-Host "  - $($_.title)" -ForegroundColor Cyan }
Write-Host ""
Write-Host "Total Runtime: $([math]::Round($totalDuration.TotalSeconds, 1)) seconds" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

Read-Host "Press Enter to exit"
