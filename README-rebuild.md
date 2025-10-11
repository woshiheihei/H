# Halt Quick Rebuild Script

## Quick Start

```powershell
# Full rebuild workflow (Stop -> Clean -> Build -> Start)
.\rebuild-halt.ps1

# Quick restart (skip build)
.\rebuild-halt.ps1 -SkipBuild

# Build only (no start)
.\rebuild-halt.ps1 -BuildOnly

# Debug build
.\rebuild-halt.ps1 -Config Debug
```

## Parameters

| Parameter | Type | Description | Default |
|-----------|------|-------------|---------|
| `-SkipBuild` | Switch | Skip compilation | false |
| `-BuildOnly` | Switch | Build only, don't start | false |
| `-Config` | String | Build configuration | Release |

**Config options**: `Release`, `Debug`, `RelWithDebInfo`

## Configuration

Edit `$Script:Settings` at the top of script:

```powershell
$Script:Settings = @{
    BuildDir     = "C:\W\HR\Slicer-build"                    # Build directory
    ExePath      = "C:\W\HR\Slicer-build\Halt.exe"           # Executable
    ConfigCache  = "C:\Users\weiba\AppData\Roaming\TAVIMERCY\Halt.ini"  # Config cache
    BuildTarget  = "qHaltApp"                                # CMake target
    WaitTime     = 2                                         # Wait time (seconds)
}
```

## First Time Setup

```powershell
# Set execution policy (one time)
Set-ExecutionPolicy -Scope CurrentUser RemoteSigned -Force
```

## Workflow

1. **Stop** - Terminate Halt processes
2. **Clean** - Delete config cache (Halt.ini)
3. **Build** - CMake compile qHaltApp (optional)
4. **Start** - Launch Halt.exe (optional)

## Examples

### Daily development
```powershell
.\rebuild-halt.ps1
```

### Config issue, need reset
```powershell
.\rebuild-halt.ps1 -SkipBuild
```

### CI/CD build
```powershell
.\rebuild-halt.ps1 -BuildOnly
```

### Debug version
```powershell
.\rebuild-halt.ps1 -Config Debug
```

## Troubleshooting

**Build fails?**
- Check CMake in PATH
- Verify build directory exists
- Check permissions

**Script blocked?**
```powershell
Set-ExecutionPolicy -Scope CurrentUser RemoteSigned -Force
```

**Help?**
```powershell
Get-Help .\rebuild-halt.ps1 -Full
```

## Extensibility

**Add custom paths:**
```powershell
$Script:Settings.CustomPath = "C:\Your\Path"
```

**Modify build target:**
```powershell
BuildTarget = "OtherTarget"
```

**Customize functions:**
- `Invoke-StopHalt` - Stop processes
- `Invoke-ClearCache` - Clear cache
- `Invoke-Build` - Build
- `Invoke-StartHalt` - Start app

---

**Version**: 1.0 | **Date**: 2025-10-11 | **Team**: TAVR Research
