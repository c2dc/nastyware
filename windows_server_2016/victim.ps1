# Variables
$PATH_RANSOMWARE = "C:\Users\Administrator\Desktop\Ransomware"

# Disable firewall from Windows
Set-NetFirewallProfile -Profile Domain,Public,Private -Enabled False

# List samples: Avaddon, Crysis, Dharma, Djvu, Gandcrab, Phobos, Revil, Teslacrypt, Troldesh, Wannacry
$SAMPLE = $args[0]

Write-Output "***** Start running victim.ps1 with sample $SAMPLE !!! *****"
mv "$PATH_RANSOMWARE\$SAMPLE.sample" "$PATH_RANSOMWARE\$SAMPLE.exe"
& "$PATH_RANSOMWARE\$SAMPLE.exe"
