#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <cjson/cJSON.h>

char *input = NULL;
char *inputf = NULL;
char *inputb = NULL;
char *sp = NULL;
char path[512];
char pathtask[512];
char pathconfig[512];
char lastinput[512];
DIR *d;

void handle_kill(int sig) {
	if (input) { free(input); input = NULL; }
    if (inputf) { free(inputf); inputf = NULL; }
    if (inputb) { free(inputb); inputb = NULL; }
	if (sp) { free(sp); sp = NULL; }
	write_history(path);
	closedir(d);
	printf("\n");
	exit(0);
}

int main(int argc, char *argv[]) {	
	char *home = getenv("HOME");
	snprintf(pathtask, sizeof(pathtask), "%s/.task", home);
	snprintf(pathconfig, sizeof(pathconfig), "%s/.task/config.json", home);
	d = opendir(pathtask);
	if (access(pathtask, F_OK) != 0) {
		system("mkdir ~/.task");
	}
	if (access(pathconfig, F_OK) != 0) {
		system("touch ~/.task/config.json");
		system("echo '{\n	\"cat\": \"glow --pager\",\n	\"edit\": \"nano\"\n}' >> ~/.task/config.json");
	}
	if (argc >= 2) {
		if (strcmp(argv[1], "raport") == 0) {
			printf("Raportuje!\n");
			if (d) {
				printf("Działa Dir: %s \n", pathtask);
			}
			return 0;
		}
	}
    snprintf(path, sizeof(path), "%s/.cache/luna-taskman", home);
	read_history(path);
	signal(SIGTERM, handle_kill);
	signal(SIGINT, handle_kill);
	while (1) {
		input = readline("> ");
		if (history_length > 0) {
			const char *line = history_get(history_length)->line;
			strncpy(lastinput, line, sizeof(lastinput));
			lastinput[sizeof(lastinput) - 1] = '\0';
		}
		if (!lastinput || strcmp(input, lastinput) != 0) {
			if (*input) {
				add_history(input);
			}
		}
		sp = strchr(input, ' ');
    	if (sp) {
        	*sp = '\0';
        	inputf = strdup(input); 
        	inputb = strdup(sp + 1); 
    	} else {
        	inputf = strdup(input); 
    		inputb = NULL;
		}
		//strncpy(lastinput, input, sizeof(lastinput));
		//lastinput[sizeof(lastinput) - 1] = '\0';	
		if (strcmp(inputf, "exit") == 0 || strcmp(inputf, "quit") == 0) {
			if (sp) free(sp);
			if (input) free(input);
			if (inputb) free(inputb);
			if (inputf) free(inputf);
			write_history(path);
			if (d) closedir(d);
			break;
		}
	}
	return 0;
}
