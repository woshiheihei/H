# Halt Development Scripts

PowerShell automation scripts for development workflow.

## Scripts

### rebuild-halt.ps1
C++ quick rebuild (~2-5 min)
```powershell
.\scripts\rebuild-halt.ps1              # Full rebuild and start
.\scripts\rebuild-halt.ps1 -BuildOnly   # Build only
.\scripts\rebuild-halt.ps1 -SkipBuild   # Quick restart
.\scripts\rebuild-halt.ps1 -Config Debug
```

### update-python-modules.ps1
Python modules update (~3-5 sec)
```powershell
.\scripts\update-python-modules.ps1
.\scripts\update-python-modules.ps1 -Config Debug
```

## Quick Reference

| Modified | Command | Time |
|----------|---------|------|
| Python code | `.\scripts\update-python-modules.ps1` | 3-5 sec |
| C++ code | `.\scripts\rebuild-halt.ps1` | 2-5 min |
