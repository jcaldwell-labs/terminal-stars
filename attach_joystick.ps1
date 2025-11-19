# Joystick USB Passthrough Helper for WSL2
# Run this in PowerShell as Administrator

Write-Host "============================================" -ForegroundColor Cyan
Write-Host " Joystick USB Passthrough for WSL2" -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan
Write-Host ""

# Check if running as administrator
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) {
    Write-Host "ERROR: This script must be run as Administrator!" -ForegroundColor Red
    Write-Host "Right-click PowerShell and select 'Run as Administrator'" -ForegroundColor Yellow
    pause
    exit 1
}

# Check if usbipd is installed
try {
    $null = Get-Command usbipd -ErrorAction Stop
} catch {
    Write-Host "ERROR: usbipd-win is not installed!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Install it with:" -ForegroundColor Yellow
    Write-Host "  winget install --interactive --exact dorssel.usbipd-win" -ForegroundColor White
    Write-Host ""
    pause
    exit 1
}

Write-Host "Step 1: Listing USB devices..." -ForegroundColor Green
Write-Host ""
usbipd list
Write-Host ""

Write-Host "Step 2: Find your joystick in the list above" -ForegroundColor Yellow
Write-Host "Look for: Xbox, PlayStation, Logitech, or other game controller" -ForegroundColor Gray
Write-Host ""
$busid = Read-Host "Enter the BUSID (e.g., 1-4)"

Write-Host ""
Write-Host "Step 3: Binding device $busid..." -ForegroundColor Green
usbipd bind --busid $busid

Write-Host ""
Write-Host "Step 4: Attaching to WSL..." -ForegroundColor Green
usbipd attach --wsl --busid $busid

Write-Host ""
Write-Host "============================================" -ForegroundColor Cyan
Write-Host " Done! Joystick should now be available" -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "To verify in WSL2, run:" -ForegroundColor Yellow
Write-Host "  ./check_joystick.sh" -ForegroundColor White
Write-Host ""
Write-Host "To detach the device:" -ForegroundColor Yellow
Write-Host "  usbipd detach --busid $busid" -ForegroundColor White
Write-Host ""

pause
