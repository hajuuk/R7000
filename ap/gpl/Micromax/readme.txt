--How to Install----------------------
1.Insert the Data card in the USB socket of your PC.
2. Double-click on the CD ROM icon displayed on the desktop. Then copy the Linux Folder from the directory displayed on clicking the the CD ROM icon.


You can use the shell command:

  "rm -rf /tmp/linux"
  "cp /media/Modem/linux/ /tmp/ -rf" or directly copy the folder"linux" in the cdrom to /tmp folder.

3.Change the "/tmp/linux/install_linux" file operate mode using the shell command:
  "cd /tmp/linux"
  "chmod a+x install_linux"
4. su as root and run "install_linux" in TERMINAL to install USBModem
	"sudo -s"
	input super user password, so now you are the supper user,run command 	install_linux to install
   	"sudo ./install_linux".

5. If you have previously installed this software in your system, you will get a prompt: "The software already exists, do you want overwrite? ([Y]/[N])", enter "Y" to overwrites or "N" to exit.

6. If you have not installed this software in your system before, you will get a prompt: "Please input the install path[/usr/local/MMX353W 3G USB Manager]:". Then you can input install path (full path), or you may use the default path[/usr/local/MMX353W 3G USB Manager] by pressing ENTER directly.   

7. After installation is complete, plug out and then plug in the USB Modem again. 
   

--How to run--------------------------
*(1) From Applications->Internet->MMX353W 3G USB Manager

*(2) Run PhonePad in your install path
   eg: # /usr/local/MMX353W 3G USB Manager/USBModem

*(3) The prefered method is to plug in your device, it will run automatically (Not supported in Xandros).

*(4) For ubuntu, you must login as root and run USBModem 
    eg: # sudo -s
        input your root password
        # /usr/local/MMX353W 3G USB Manager/USBModem 
        or
        # /usr/local/MMX353W 3G USB Manager/USBModem.sh

--How to Uninstall----------------------
*You need login as root*
   Run UninstallUSBModem in TERMINAL to Uninstall USBModem
   eg: # /usr/local/MMX353W 3G USB Manager/UninstallUSBModem

--After connecting to the Internet why cannot I view websites via firefox?----------------

Ans: Start firefox and check File->Work Offline checked or not,if checked then uncheck it,you can view website immediately


--if you can not install in fedora with the error "gnome-terminal"------------------
try to exit super user with command "exit",and re enter into root with command "su - root" but not "su root"

--how to install software on ubuntu 10.04 x86_64 11.04 x86_64 12.04 x86_64-------
before installing the software, you must install dependent library that will be used by the software.
1.update apt source list
# sudo apt-get update
2.install libc lib
# sudo apt-get install g++-multilib
3.install X printing Extension (Xprint) client library
# sudo apt-get install libxp-dev
# sudo apt-get install ia32-libs
4.install libusb-dev  ubuntu

--for improve software stability please upgrade linux library on ubuntu-------
1.update apt source list
# sudo apt-get update
2.upgrade all linux patches
# sudo apt-get upgrade

how to root in shell
ubuntu:
$sudo -s
input super user password.

--how to support voice call-----------
ubuntu:
udpate and install alsa about library.

--why I cannot open help document--------
ubuntu:
$sudo apt-get install evince

