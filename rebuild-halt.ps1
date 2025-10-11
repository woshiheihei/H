<#
.SYNOPSIS
    Halt App Quick Rebuild Script

.DESCRIPTION
    One-click workflow: Stop -> Clean -> Build -> Start
    
.PARAMETER SkipBuild
    Skip compilation, only clean and start

.PARAMETER BuildOnly
    Only build, do not start

.PARAMETER Config
    Build configuration: Debug, Release (default), RelWithDebInfo

.EXAMPLE
    .\rebuild-halt.ps1
    # Full rebuild

.EXAMPLE
    .\rebuild-halt.ps1 -SkipBuild
    # Quick restart (skip build)

.EXAMPLE
    .\rebuild-halt.ps1 -BuildOnly
    # Build only

.EXAMPLE
    .\rebuild-halt.ps1 -Config Debug
    # Debug build
#>

[CmdletBinding()]
param(
    [switch]$SkipBuild,
    [switch]$BuildOnly,
    [ValidateSet("Debug", "Release", "RelWithDebInfo")]
    [string]$Config = "Release"
)

# Configuration - Modify paths as needed
$Script:Settings = @{
    BuildDir     = "C:\W\HR\Slicer-build"
    ExePath      = "C:\W\HR\Slicer-build\Halt.exe"
    ConfigCache  = "C:\Users\weiba\AppData\Roaming\TAVIMERCY\Halt.ini"
    BuildTarget  = "qHaltApp"
    WaitTime     = 2
}

# Helper functions
function Write-ColorMsg { param($Msg, $Color = "White") Write-Host $Msg -ForegroundColor $Color }
function Write-Section { param($Title) Write-Host "`n$('='*60)" -ForegroundColor Cyan; Write-Host $Title -ForegroundColor Cyan; Write-Host "$('='*60)`n" -ForegroundColor Cyan }
function Write-Success { Write-ColorMsg "[OK] $args" Green }
function Write-Info { Write-ColorMsg "     $args" Gray }
function Write-Warning { Write-ColorMsg "[!!] $args" Yellow }
function Write-Error { Write-ColorMsg "[ERR] $args" Red }

# Core functions
function Invoke-StopHalt {
    Write-Section "STEP 1: Stop Halt Processes"
    $processes = Get-Process -Name "Halt", "HaltApp-real" -ErrorAction SilentlyContinue
    if ($processes) {
        $processes | ForEach-Object { Write-Info "$($_.ProcessName) (PID: $($_.Id))" }
        Stop-Process -Name "Halt", "HaltApp-real" -Force -ErrorAction SilentlyContinue
        Write-Success "Stopped all Halt processes"
        Start-Sleep -Seconds $Script:Settings.WaitTime
    } else {
        Write-Success "No running processes"
    }
}

function Invoke-ClearCache {
    Write-Section "STEP 2: Clear Config Cache"
    if (Test-Path $Script:Settings.ConfigCache) {
        Remove-Item $Script:Settings.ConfigCache -Force -ErrorAction SilentlyContinue
        Write-Success "Cache deleted"
    } else {
        Write-Success "Cache not found"
    }
}

function Invoke-Build {
    Write-Section "STEP 3: Build $($Script:Settings.BuildTarget) [$Config]"
    if (-not (Test-Path $Script:Settings.BuildDir)) {
        Write-Error "Build directory not found: $($Script:Settings.BuildDir)"
        exit 1
    }
    
    Push-Location $Script:Settings.BuildDir
    try {
        Write-Info "cmake --build . --target $($Script:Settings.BuildTarget) --config $Config`n"
        
        # Use Start-Process with -Wait and -NoNewWindow for real-time output
        $process = Start-Process -FilePath "cmake" `
            -ArgumentList "--build", ".", "--target", $Script:Settings.BuildTarget, "--config", $Config `
            -Wait -NoNewWindow -PassThru
        
        if ($process.ExitCode -eq 0) {
            Write-Success "Build succeeded"
        } else {
            Write-Error "Build failed (Exit Code: $($process.ExitCode))"
            Pop-Location
            exit $process.ExitCode
        }
    } finally {
        Pop-Location
    }
}

function Invoke-StartHalt {
    Write-Section "STEP 4: Start Halt App"
    if (-not (Test-Path $Script:Settings.ExePath)) {
        Write-Error "Executable not found: $($Script:Settings.ExePath)"
        exit 1
    }
    
    try {
        Start-Process $Script:Settings.ExePath
        Write-Success "Application started"
        Write-Info $Script:Settings.ExePath
    } catch {
        Write-Error "Failed to start: $_"
        exit 1
    }
}

# Main workflow
Clear-Host
Write-ColorMsg "`n  HALT App Quick Rebuild Tool v1.0  `n" Magenta

# Display mode
$mode = if ($SkipBuild) { "Quick Restart" } elseif ($BuildOnly) { "Build Only" } else { "Full Rebuild" }
Write-ColorMsg "Mode: $mode" Yellow
$startTime = Get-Date

try {
    # Step 1: Stop processes
    Invoke-StopHalt
    
    # Step 2: Clear cache
    Invoke-ClearCache
    
    # Step 3: Build (optional)
    if (-not $SkipBuild) {
        Invoke-Build
    } else {
        Write-Warning "Skipping build"
    }
    
    # Step 4: Start (optional)
    if (-not $BuildOnly) {
        Invoke-StartHalt
    } else {
        Write-Warning "Skipping start"
    }
    
    # Done
    $duration = (Get-Date) - $startTime
    Write-ColorMsg "`n$('='*60)" Green
    Write-Success "All operations completed! Time: $($duration.Minutes)m $($duration.Seconds)s"
    Write-ColorMsg "$('='*60)`n" Green
    
} catch {
    Write-Error "Execution failed: $_"
    exit 1
}
