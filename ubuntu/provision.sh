#!/bin/bash

echo "***** Starting execution of provision.sh!!!"

apt update -y && apt upgrade -y

apt install -y ssh

service ssh enable



### Configure python 2 virtual environment
apt-get update
apt-get install python2 curl
cd /tmp
curl https://bootstrap.pypa.io/get-pip.py --output get-pip.py
python2 get-pip.py
pip2 install virtualenv
mkdir ~/virtualenv
cd ~/virtualenv
virtualenv damicore
source virtualenv/damicore/bin/activate
pip install figtree