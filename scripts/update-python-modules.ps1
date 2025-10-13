<#
.SYNOPSIS
    Quick update Python scripted modules only

.DESCRIPTION
    Fast deployment of Python module changes without full rebuild.
    This script builds only the tavi_analytics extension, which copies
    updated Python files to the runtime directory.

.PARAMETER Config
    Build configuration: Debug, Release (default), RelWithDebInfo

.EXAMPLE
    .\update-python-modules.ps1
    # Update Python modules (Release build)

.EXAMPLE
    .\update-python-modules.ps1 -Config Debug
    # Update Python modules (Debug build)
#>

param(
    [ValidateSet("Debug", "Release", "RelWithDebInfo")]
    [string]$Config = "Release"
)

# Configuration
$Script:Settings = @{
    ExtensionBuildDir = "C:\W\HR\Slicer-build\E\tavi_analytics"
    SourceDir        = "C:\W\H\Modules\Scripted\tavi_analytics"
    RuntimeDir       = "C:\W\HR\Slicer-build\lib\Halt-5.9\qt-scripted-modules"
}

# Helper functions
function Write-ColorMsg { param($Msg, $Color = "White") Write-Host $Msg -ForegroundColor $Color }
function Write-Success { Write-ColorMsg "[OK] $args" Green }
function Write-Info { Write-ColorMsg "     $args" Gray }
function Write-Error { Write-ColorMsg "[ERR] $args" Red }

# Main
Clear-Host
Write-ColorMsg "`n  Python Modules Quick Update Tool  `n" Magenta
Write-ColorMsg "Config: $Config" Yellow

$startTime = Get-Date

# Check if extension build directory exists
if (-not (Test-Path $Script:Settings.ExtensionBuildDir)) {
    Write-Error "Extension build directory not found: $($Script:Settings.ExtensionBuildDir)"
    Write-Info "Please run a full build first."
    exit 1
}

Write-Info "Building tavi_analytics extension...`n"

Push-Location $Script:Settings.ExtensionBuildDir
try {
    # Build the extension (this copies Python files to runtime directory)
    $process = Start-Process -FilePath "cmake" `
        -ArgumentList "--build", ".", "--config", $Config `
        -Wait -NoNewWindow -PassThru
    
    if ($process.ExitCode -ne 0) {
        Write-Error "Build failed (Exit Code: $($process.ExitCode))"
        Pop-Location
        exit $process.ExitCode
    }
    
    Write-Success "Build completed"
    
    # Verify files were updated
    Write-Info "`nVerifying file updates..."
    
    $sampleFile = Join-Path $Script:Settings.RuntimeDir "tavi_analytics.py"
    if (Test-Path $sampleFile) {
        $lastWrite = (Get-Item $sampleFile).LastWriteTime
        $age = (Get-Date) - $lastWrite
        
        if ($age.TotalMinutes -lt 1) {
            Write-Success "Files updated successfully ($('{0:N0}' -f $age.TotalSeconds) seconds ago)"
        } else {
            Write-Info "Last update: $lastWrite"
        }
    }
    
    # Done
    $duration = (Get-Date) - $startTime
    Write-ColorMsg "`n$('='*60)" Green
    Write-Success "All done! Time: $($duration.Minutes)m $($duration.Seconds)s"
    Write-ColorMsg "$('='*60)`n" Green
    Write-Info "Runtime directory: $($Script:Settings.RuntimeDir)"
    
} catch {
    Write-Error "Execution failed: $_"
    exit 1
} finally {
    Pop-Location
}
