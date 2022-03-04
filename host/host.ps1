# Variables
$CONFIG_VICTIM = "G:\Virtual Machines\Win2k16\Windows Server 2016.vmx"
$LOG_FILE = "host-nastware.log"
$PATH_RANSOMWARE = "C:\Users\Administrator\Desktop\Ransomware"
$SCRIPT_NAME = "victim.ps1"
$SNAPSHOT_NAME = "Snapshot-ransomware"
$VM_ATTACK = "Kali-2020.1"
$GUEST_USER = "Administrator"
$GUEST_PASS = 'Pa$$w0rd'
$VM_RUN = "C:\Program Files (x86)\VMware\VMware Workstation\vmrun.exe"


# Install modules
Install-Module -Name vmxtoolkit
# Install-Module VMware.PowerCLI -Scope CurrentUser


# Select samples
# $SAMPLES = @()
# Get-ChildItem -Path C:\sample\ransomware -Filter *.sample -File | ForEach-Object {
#     $SAMPLES += [io.path]::GetFileNameWithoutExtension($_)
# }
# Write-Output "$($SAMPLES)"

# If you want to run only one sample.
# $SAMPLES = @("Wannacry")

# If you want to run specifics samples.
# $SAMPLES = @("Avaddon", "Crysis", "Dharma", "Djvu", "Gandcrab", "Phobos", "Revil", "Teslacrypt", "Troldesh", "Wannacry")
$SAMPLES = @("131", "Avaddon", "cerber", "cryptowall", "Crysis", "Dharma", "Djvu", "Gandcrab", "jigsaw", "Locky", "Petwrap", "Petya", "Phobos", "Radamant", "RedBoot", "Revil", "Satana", "Teslacrypt", "Thanos", "Troldesh", "Unnamed", "Vipasana", "Wannacry", "Win32.Wannacry")

# Run ransomwares from host to victim (VM Windows Server 2016)
for ($i=0; $i -lt $SAMPLES.length; $i++) {

    Restore-VMXSnapshot -config $CONFIG_VICTIM -Snapshot $SNAPSHOT_NAME
    Write-Output "**** Virtual machine restored at $(Get-Date)." | Tee-Object $LOG_FILE -Append

    Start-VMX -config $CONFIG_VICTIM
    Write-Output "**** Virtual machine started at $(Get-Date)." | Tee-Object $LOG_FILE -Append

    Start-Sleep -s 300
    Write-Output "**** Sample $($SAMPLES[$i]) started at $(Get-Date). Waiting 5 minutes..." | Tee-Object $LOG_FILE -Append
    
    &$VM_RUN -gu $GUEST_USER -gp $GUEST_PASS runPrograminGuest $CONFIG_VICTIM -activewindow "$nowait_parm" $interactive_parm "c:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe" -Executionpolicy bypass "$PATH_RANSOMWARE\$SCRIPT_NAME" $($SAMPLES[$i])
    Write-Output "**** Sample $($SAMPLES[$i]) executed at $(Get-Date). Waiting 10 minutes..." | Tee-Object $LOG_FILE -Append
    Start-Sleep -s 600
    Write-Output "**** Execution of sample $($SAMPLES[$i]) finished at $(Get-Date)." | Tee-Object $LOG_FILE -Append
}

Restore-VMXSnapshot -config $CONFIG_VICTIM -Snapshot $SNAPSHOT_NAME
Write-Output "**** Virtual machine restored for the last time at $(Get-Date)." | Tee-Object $LOG_FILE -Append