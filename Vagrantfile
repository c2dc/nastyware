# _*_ mode: ruby _*_
# vi: set ft=ruby :

Vagrant.configure("2") do |config|

  # Windows Server 2016 (victim)
  config.vm.define "windows_server_2016" do |windows_server_2016|
    windows_server_2016.vm.box = "StefanScherer/windows_2016"
    windows_server_2016.vm.hostname = "Server2016"
    windows_server_2016.vm.synced_folder ".", "/vagrant", disabled: true
    windows_server_2016.vm.network "private_network", ip: "192.168.56.16/24"
    windows_server_2016.vm.provision "shell", privileged: "false", path: "windows_server_2016/provision.ps1" #powershell_elevated_interactive: "true",
    windows_server_2016.vm.provider "vmware_desktop" do |v|
      v.vmx["memsize"] = "8192"
      v.vmx["numvcpus"] = "4"
    end
  end

  # Ubuntu (victim linux)
  config.vm.define "ubuntu" do |ubuntu|
    ubuntu.vm.box = "bento/ubuntu-20.04"
    ubuntu.vm.hostname = "user"
    ubuntu.vm.synced_folder ".", "/vagrant", disabled: true
    ubuntu.vm.network "private_network", ip: "192.168.56.13/24"
    ubuntu.vm.provision "shell", run: "always", path: "ubuntu/provision.sh"
    ubuntu.vm.provider "vmware_desktop" do |v|
      v.vmx["memsize"] = "3072"
      v.vmx["numvcpus"] = "2"
    end
  end

  # Wazuh server
  # config.vm.define "wazuh" do |wazuh|
  #   wazuh.vm.box = "uahccre/wazuh-manager"
  #   wazuh.vm.hostname = "wazuh"
  #   wazuh.vm.synced_folder ".", "/vagrant", disabled: true
  #   wazuh.vm.network "private_network", ip: "192.168.56.234/24"
  #   wazuh.vm.provision "shell", run: "always", path: "wazuh/provision.sh"
  #   wazuh.vm.provider "virtualbox" do |v|
  #     v.vmx["memsize"] = "16384"
  #     v.vmx["numvcpus"] = "4"
  #   end
  # end

  # Kali (attacker)
  config.vm.define "attacker" do |attacker|
    attacker.vm.box = "kalilinux/rolling" 
    attacker.vm.hostname = "user"
    attacker.vm.synced_folder ".", "/vagrant", disabled: true
    attacker.vm.network "private_network", ip: "192.168.56.3/24"
    attacker.vm.provision "shell", run: "always", path: "kali/provision.sh"
    attacker.vm.provider "vmware_desktop" do |v|
      v.vmx["memsize"] = "3072"
      v.vmx["numvcpus"] = "2"
    end
  end

end