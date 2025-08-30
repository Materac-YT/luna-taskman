# Luna-Taskman  
Is FOSS made for linux, i belive it probably could work on UNIX based.

# How To Install Or Update?  
## Install Dependencies:  
### On Debian Based:  
Type in terminal to install  
'sudo apt install libreadline-dev libncurses-dev libcjson-dev'  
### On Fedora Based:  
Type in terminal to install  
'sudo dnf install readline-devel ncurses-devel cjson-devel'  
### On Arch Based:  
Type in terminal to install  
'sudo pacman -S readline ncurses cjson'  
## Download Source Code  
Type in terminal to download  
'git clone https://github.com/Materac-YT/luna-taskman.git'  
'cd luna-taskman'  
## Compile  
Type in terminal to check is dependencis are installed  
'make check'  
Type in terminal to build from sources  
'make build'  
Type in terminal to install  
'make install'  
## Delete Source Code  
Type in terminal  
'cd ..'  
'rm -r luna-taskman'

# How To Uninstall?  
## Download Source Code  
Type in terminal to download  
'git clone https://github.com/Materac-YT/luna-taskman.git'  
'cd luna-taskman'  
## Uninstall  
Type in terminal to uninstall  
'make uninstall'  
## Delete Source Code  
TYpe in terminal  
'cd ..'  
'rm -r luna-taskman'  
## To Delete All Task  
Type in terminal  
'rm -r ~/.task'  
## To Delete Program History  
Type in terminal  
'rm ~/.cache/luna-taskman'

# How To Use?  
## To Enter Interactive Mode  
Type in terminal  
'luna-taskmanctl'  
## How To Use Interactive Mode  
Type in interactive mode  
'help'  
This will show how to use program  
## How To Use Raport Function?  
Type in terminal  
'luna-taskmanctl raport'  
This will show all task to do or in work in all groups without entering interactive mode  
