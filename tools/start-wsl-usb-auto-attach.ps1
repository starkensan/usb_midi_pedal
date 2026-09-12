[CmdletBinding()]
param(
    [Parameter(Mandatory)]
    [ValidatePattern("^\d+-\d+$")]
    [string]$BusId
)

$ErrorActionPreference = "Stop"

$usbipd = Get-Command -Name "usbipd" -CommandType Application -ErrorAction SilentlyContinue
if ($null -eq $usbipd) {
    throw "usbipd was not found. Install usbipd-win before running this script."
}

Write-Host "Monitoring USB BUSID $BusId for WSL auto-attach."
Write-Host "Keep this PowerShell window open. Press Ctrl+C to stop monitoring."

& $usbipd.Path attach --wsl --busid $BusId --auto-attach
exit $LASTEXITCODE
