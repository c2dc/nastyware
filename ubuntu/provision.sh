#!/bin/bash

echo "***** Starting execution of provision.sh!!!"

apt update -y && apt upgrade -y

apt install -y ssh

service ssh enable

