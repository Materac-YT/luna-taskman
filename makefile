default: full
full:
	@ make check || exit 1
	@ make build || exit 1
	@ make install || exit 1
build:
	gcc src/task.c -o task.x -lreadline -lhistory -lncurses -lcjson
install:
	sudo install -Dm755 task.x /bin/luna-taskmanctl
	sudo mkdir -p -m 755 /usr/share/doc/luna-taskman
	sudo mkdir -p -m 755 /usr/share/licenses/luna-taskman
	sudo install -Dm755 LICENSE /usr/share/doc/luna-taskman/LICENSE
	sudo install -Dm755 COPYRIGHT /usr/share/doc/luna-taskman/COPYRIGHT
	sudo install -Dm755 LICENSE /usr/share/licenses/luna-taskman/LICENSE
	sudo install -Dm755 COPYRIGHT /usr/share/licenses/luna-taskman/COPYRIGHT
	@ echo To use this program use comand \"luna-taskmanctl\"
uninstall:
	sudo rm -r /bin/luna-taskmanctl /usr/share/doc/luna-taskman /usr/share/licenses/luna-taskman
check:
	@ find /usr/lib /usr/local/lib -name "libcjson.so*" &>/dev/null || { echo "There is no readline installed. To install: \nOn Debian based: sudo apt install libcjson-dev \nOn Fedora based: sudo dnf install cjson-devel \nOn Arch based: sudo pacman -S cjson"; exit 1; }
	@ find /usr/include /usr/local/include -name "cjson.h" &>/dev/null || { echo "There is no readline installed. To install: \nOn Debian based: sudo apt install libcjson-dev \nOn Fedora based: sudo dnf install cjson-devel \nOn Arch based: sudo pacman -S cjson"; exit 1; }
	@ find /usr/lib /usr/local/lib -name "libreadline.so*" &>/dev/null || { echo "There is no readline installed. To install: \nOn Debian based: sudo apt install libreadline-dev \nOn Fedora based: sudo dnf install readline-devel \nOn Arch based: sudo pacman -S readline"; exit 1; }
	@ find /usr/include /usr/local/include -name "readline/readline.h" &>/dev/null || { echo "There is no readline installed. To install: \nOn Debian based: sudo apt install libreadline-dev \nOn Fedora based: sudo dnf install readline-devel \nOn Arch based: sudo pacman -S readline"; exit 1; }
	@ find /usr/lib /usr/local/lib -name "libncurses.so*" &>/dev/null || { echo "There is no ncurses installed. To install: \nOn Debian based: sudo apt install libncurses-dev \nOn Fedora based: sudo dnf install ncurses-devel \nOn Arch based: sudo pacman -S ncurses"; exit 1; }
	@ find /usr/include /usr/local/include -name "ncurses.h" &>/dev/null || { echo "There is no ncurses installed. To install: \nOn Debian based: sudo apt install libncurses-dev \nOn Fedora based: sudo dnf install ncurses-devel \nOn Arch based: sudo pacman -S ncurses"; exit 1; }
	@ which nano &> /dev/null || echo "There is no nano installed which is default text editor to edit task info, to change type in \"luna-taskmanctl\" and in interactive mode type in \"config\" and follow instructions. To install: \nOn Debian based: sudo apt install nano \nOn Fedora based: sudo dnf install nano \nOn Arch based: sudo pacman -S nano"
	@ which glow &> /dev/null || echo "There is no glow installed which is default md version of cat to cat task info, to change type in \"luna-taskmanctl\" and in interactive mode type in \"config\" and follow instructions. To install: \nOn Debian based: sudo apt install glow \nOn Fedora based: sudo dnf install glow \nOn Arch based: sudo pacman -S glow"
