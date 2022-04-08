Write-Host "***** Starting execution of provision.sh..."

Write-Host "***** Elevating privileges..."
if (!([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) { Start-Process powershell.exe "-NoProfile -ExecutionPolicy Bypass -File `"$PSCommandPath`"" -Verb RunAs; exit }

Write-Host "Configuring packages to run damicore..."
pip install figtree python-igraph

Write-Host "Installing yara..."
choco install yara -y
