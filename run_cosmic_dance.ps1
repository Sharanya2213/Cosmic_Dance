# ============================================================================
# COSMIC DANCE - SEQUENTIAL ANIMATION LAUNCHER
# Runs all 16 chapter animations in sequence (5-6 minutes total)
# FULLSCREEN + CTRL+C HANDLER FOR ALL PROCESSES
# ============================================================================

# Store all running processes for cleanup
$script:runningProcesses = @()

# Setup Ctrl+C handler to kill everything
$ErrorActionPreference = "Continue"

function Cleanup {
    Write-Host ""
    Write-Host "=========================================="
    Write-Host "TERMINATING ALL PROCESSES..."
    Write-Host "=========================================="
    
    # Kill all running animation processes
    Get-Process | Where-Object {$_.Name -like "*.exe"} | ForEach-Object {
        try {
            Stop-Process -Id $_.Id -Force -ErrorAction SilentlyContinue
            Write-Host "Killed: $($_.Name)"
        } catch {}
    }
    
    Write-Host "All processes terminated."
    Write-Host "=========================================="
    exit 0
}

# Trap for Ctrl+C
$null = Register-EngineEvent -SourceIdentifier PowerShell.Exiting -Action { Cleanup }

# TIMING CONFIGURATION (in seconds) - EDIT THESE VALUES
# Aim for total between 300-360 seconds (5-6 minutes)
# Starting from Chapter 1 (Chaos) through Chapter 17 (Humanity)
$chapters = @(
    @{name="Chapter 1: Chaos and Darkness"; exe="chaos_darkness.exe"; duration=20},
    @{name="Chapter 2: The Big Bang"; exe="chapter2_bigbang.exe"; duration=22},
    @{name="Chapter 3: Nebula Genesis"; exe="nebula_genesis.exe"; duration=22},
    @{name="Chapter 4: Cosmic Observer"; exe="cosmic_observer.exe"; duration=22},
    @{name="Chapter 5: Galaxy Milkyway"; exe="galaxy_milkyway.exe"; duration=22},
    @{name="Chapter 6: Accurate Solar System"; exe="chapter3_accurate_solar.exe"; duration=25},
    @{name="Chapter 7: Emergence of Life"; exe="emergence_of_life.exe"; duration=22},
    @{name="Chapter 8: First Cells"; exe="first_cells.exe"; duration=22},
    @{name="Chapter 9: Multicellular Organism"; exe="multicellular_organism.exe"; duration=35},
    @{name="Chapter 10: Early Organism"; exe="chapter10_early_organism.exe"; duration=22},
    @{name="Chapter 11: Complex Organism"; exe="chapter11_complex_organism.exe"; duration=22},
    @{name="Chapter 12: Ocean to Land"; exe="chapter12_ocean_to_land.exe"; duration=22},
    @{name="Chapter 13: Land Volcanos"; exe="chapter13_land_volcanos.exe"; duration=22},
    @{name="Chapter 14: Dinosaur Era"; exe="chapter14_dinosaur_era.exe"; duration=25},
    @{name="Chapter 15: Mammalian Rise"; exe="chapter15_mammalian_rise.exe"; duration=25},
    @{name="Chapter 17: Humanity Observes"; exe="chapter17_humanity_observes.exe"; duration=22}
)

# Calculate total duration
$totalDuration = 0
foreach ($ch in $chapters) {
    $totalDuration += $ch.duration
}

Write-Host "=========================================="
Write-Host "COSMIC DANCE - ANIMATION SEQUENCE"
Write-Host "=========================================="
Write-Host "Total Duration: $totalDuration seconds (~$(([math]::Round($totalDuration/60, 1))) minutes)"
Write-Host ""
Write-Host "Chapters to play:"
for ($i = 0; $i -lt $chapters.Count; $i++) {
    Write-Host "  $($i): $($chapters[$i].name) ($($chapters[$i].duration)s)"
}
Write-Host ""
Write-Host "Press ENTER to start, or Ctrl+C to cancel..."
Read-Host

# Track overall time
$overallStartTime = Get-Date
$chapterIndex = 0
$loopCount = 0

# Infinite loop - runs continuously
while ($true) {
    $loopCount++
    Write-Host ""
    Write-Host "========== LOOP $loopCount =========="
    
    # Run each chapter
    foreach ($ch in $chapters) {
        $exePath = ".\$($ch.exe)"
        
        # Check if exe exists
        if (-not (Test-Path $exePath)) {
            Write-Host "⚠️  Skipping: $($ch.name) - File not found: $($ch.exe)"
            continue
        }
        
        $chapterNumber = $chapterIndex + 1
        Write-Host ""
        Write-Host "[$chapterNumber/$($chapters.Count)] Starting: $($ch.name) ($($ch.duration)s)"
        
        # Start the executable in normal window
        try {
            $process = Start-Process -FilePath $exePath -PassThru -ErrorAction Stop
            $startTime = Get-Date
            
            # Wait with timeout - check every 100ms for responsiveness
            while ((-not $process.HasExited) -and ((Get-Date) - $startTime).TotalSeconds -lt $ch.duration) {
                Start-Sleep -Milliseconds 100
            }
            
            # Kill if still running
            if (-not $process.HasExited) {
                try {
                    $process | Stop-Process -Force -ErrorAction SilentlyContinue
                    Write-Host "Auto-closed: $($ch.name)"
                } catch {}
            } else {
                Write-Host "Completed: $($ch.name)"
            }
        } catch {
            Write-Host "Error: Could not start $($ch.name)"
        }
        
        # Tiny pause between chapters
        Start-Sleep -Milliseconds 200
        $chapterIndex++
    }
    
    # Reset chapter index for next loop
    $chapterIndex = 0
    
    Write-Host ""
    Write-Host "=========================================="
    Write-Host "Loop $loopCount Complete - Restarting..."
    Write-Host "Press Ctrl+C to exit"
    Write-Host "=========================================="
    Write-Host ""
    
    # Short pause before restarting
    Start-Sleep -Milliseconds 500
}
