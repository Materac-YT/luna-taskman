default: normal
normal:
	gcc src/task.c -o task.x -lreadline -lhistory -lncurses
