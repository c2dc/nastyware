#!/bin/bash

echo "***** Starting execution of provision.sh..."

echo "***** Downloading files from repository..."
wget "https://github.com/c2dc/nastyware/blob/main/kali/attack-1.sh"
wget "https://github.com/c2dc/nastyware/blob/main/kali/attack-2.sh"
wget "https://github.com/c2dc/nastyware/blob/main/kali/wordlist.txt"

echo "***** Unziping rockyou..."
sudo gunzip /usr/share/wordlists/rockyou.txt.gz