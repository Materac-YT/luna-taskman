#default: normal
build:
	gcc src/task.c -o task.x -lreadline -lhistory -lncurses -lcjson
install:
	echo sudo install -Dm755 task.x /bin/luna-taskmanctl
	@ echo To use this program use comand \"luna-taskmanctl\"
test:
	io || echo 123
