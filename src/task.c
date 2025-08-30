/*
 * This file is part of luna-taskman
 *
 * luna-taskman is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * luna-taskman is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2025 Materac_YT
 *Copyright (C) 2025 Materac
 *(I have two nick names)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <cjson/cJSON.h>

char *input = NULL;
char *inputf = NULL;
char *inputb = NULL;
char *sp = NULL;
char *group = NULL;
char path[512];
char pathtask[512];
char pathconfig[512];
char lastinput[512];
DIR *d;
struct dirent *entry;
struct stat st;
FILE *f;
char *data;
cJSON *json;
cJSON *cat;
cJSON *edit;

void handle_kill(int sig) {
	if (input) { free(input); input = NULL; }
    if (inputf) { free(inputf); inputf = NULL; }
    if (inputb) { free(inputb); inputb = NULL; }
	if (sp) { free(sp); sp = NULL; }
	if (group) { free(group); group = NULL; }
	if (d) closedir(d);
	write_history(path);
	if (entry) free(entry);
	cJSON_Delete(json);
	closedir(d);
	printf("\n");
	exit(0);
}

void fix_spaces(char *str) {
	char *src = str;
	char *dst = str;

	while (isspace((unsigned char)*src)) {
		src++;
	}

	int space_flag = 0;
	while (*src) {
		if (isspace((unsigned char)*src)) {
			if (!space_flag) {
				*dst++ = ' ';
				space_flag = 1;
			}
		} else {
			*dst++ = *src;
			space_flag = 0;
		}
		src++;
	}

	if (dst > str && *(dst - 1) == ' ') {
		dst--;
	}

	*dst = '\0';
}

int main(int argc, char *argv[]) {
	group = strdup("none");
	char *home = getenv("HOME");
	snprintf(pathtask, sizeof(pathtask), "%s/.task", home);
	snprintf(pathconfig, sizeof(pathconfig), "%s/.task/config.json", home);
	f = fopen(pathconfig, "r");
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	rewind(f);
	data = malloc(len + 1);
	fread(data, 1, len, f);
	data[len] = '\0';
	fclose(f);
	json = cJSON_Parse(data);
	free(data);
	cat = cJSON_GetObjectItemCaseSensitive(json, "cat");
	edit = cJSON_GetObjectItemCaseSensitive(json, "edit");
	d = opendir(pathtask);
	if (access(pathtask, F_OK) != 0) {
		system("mkdir ~/.task");
	}
	if (access(pathconfig, F_OK) != 0) {
		system("touch ~/.task/config.json");
		system("echo '{\n	\"cat\": \"glow --pager\",\n	\"edit\": \"nano\"\n}' >> ~/.task/config.json");
		printf("exit - to exit ctl\naddgrp foo - to add new group named foo\ndelgrp foo - to delete group foo\nchgrp foo - changes working group to foo\naddtask foo - to add new task named foo in working group\ndeltask foo - to delete task foo in working group\ninfotask foo - shows information about task foo in working group\nmodtask foo - edits informations about task foo in working group\ndonetask foo - flags task foo as done\ninworktask foo - flags task foo as in work\ntodotask foo - flags task foo as un done\nlsgrp - list all groups\nlstask - list all task in working group\nconfig - alows to configure programs to open and edit tasks\nhelp - shows this menu\n");
	}
	if (argc >= 2) {
		if (strcmp(argv[1], "raport") == 0) {
			while ((entry = readdir(d)) != NULL) {
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
					continue;
				char fullpath[1024];
				snprintf(fullpath, sizeof(fullpath), "%s/%s", pathtask, entry->d_name);
				if (stat(fullpath, &st) == 0 && S_ISDIR(st.st_mode)) {
					printf("  Group: %s\n", entry->d_name);
					char todo[1024]; 
					snprintf(todo, sizeof(todo), "find '%s/.task/%s/todo' -maxdepth 1 -type f -name '*.md' -printf '%%f\\n' | sed 's/\\.md$//'", home, entry->d_name);
					printf(" To Do: \n");
					system(todo);
					char inwork[1024];
					snprintf(inwork, sizeof(inwork), "find '%s/.task/%s/inwork' -maxdepth 1 -type f -name '*.md' -printf '%%f\\n' | sed 's/\\.md$//'", home, entry->d_name);
					printf(" In Work: \n");
					system(inwork);
				}
			}
			if (d) closedir(d);
			if (input) { free(input); input = NULL; }
			if (inputf) { free(inputf); inputf = NULL; }
			if (inputb) { free(inputb); inputb = NULL; }
			if (sp) { free(sp); sp = NULL; }
			if (group) { free(group); group = NULL; }
			return 0;
		}
	}
    snprintf(path, sizeof(path), "%s/.cache/luna-taskman", home);
	read_history(path);
	signal(SIGTERM, handle_kill);
	signal(SIGINT, handle_kill);
	while (1) {
		char prompt[100];
		printf("\n");
		snprintf(prompt, sizeof(prompt), "Group: \"%s\" >>> ", group);
		input = readline(prompt);
		fix_spaces(input);
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
		if (strcmp(inputf, "exit") == 0 || strcmp(inputf, "quit") == 0) {
			if (sp) { free(sp); }
			if (input) { free(input); }
			if (inputf) { free(inputf); }
			if (inputb) { free(inputb); }
			if (group) { free(group); }
			write_history(path);
			if (d) closedir(d);
			if (entry) free(entry);
			cJSON_Delete(json);
			break;
		} else if (strcmp(inputf, "addgrp") == 0) {
			if (inputb && inputb[0] != '\0') {
				if (strcmp(inputb, "none") != 0) {
					char tmp[512];
					snprintf(tmp, sizeof(tmp), "mkdir -p ~/.task/'%s'", inputb);
					system(tmp);
					snprintf(tmp, sizeof(tmp), "mkdir -p ~/.task/'%s'/todo", inputb);
					system(tmp);
					snprintf(tmp, sizeof(tmp), "mkdir -p ~/.task/'%s'/inwork", inputb);
					system(tmp);
					snprintf(tmp, sizeof(tmp), "mkdir -p ~/.task/'%s'/done", inputb);
					system(tmp);
					snprintf(tmp, sizeof(tmp), "mkdir -p ~/.task/'%s'/note", inputb);
					system(tmp);
				} else {
					printf("%s is illegal group name", inputb);
				}
			} else {
				printf("%s requires an argument\n", inputf);
			}
		} else if (strcmp(inputf, "delgrp") == 0) {
			if (inputb && inputb[0] != '\0') {
				char tmp[512];
				snprintf(tmp, sizeof(tmp), "rm -rf ~/.task/'%s'", inputb);
				system(tmp);
				if (strcmp(inputb, group) == 0) {
					free(group);
					group = strdup("none");
				}
			} else {
				printf("%s requires an argument\n", inputf);
			}
		} else if (strcmp(inputf, "chgrp") == 0) {
			if (inputb && inputb[0] != '\0') {
				char tmp[512];
				snprintf(tmp, sizeof(tmp), "%s/.task/%s", home, inputb);
				if (access(tmp, F_OK) == 0) { 
					free(group);
					group = strdup(inputb);
				} else {
					printf("Group %s dosen\'t exist\n", inputb);
				}
			} else {
				printf("%s requires an argument\n", inputf);
			}
		} else if (strcmp(inputf, "addtask") == 0) {
			if (strcmp(group, "none") != 0) {
				if (inputb && inputb[0] != '\0') {
					char tmp[512];
					snprintf(tmp, sizeof(tmp), "%s/.task/%s", home, inputb);
					if (access(tmp, F_OK) == 0) {
						char tmp[512];
						snprintf(tmp, sizeof(tmp), "touch '%s/.task/%s/todo/%s.md'", home, group, inputb);
						system(tmp);
					} else {
						printf("Group %s dosen\'t exist\n", inputb);
					}
				} else {
					printf("%s requires an argument\n", inputf);
				}
			} else {
				printf("No group selected\n");
			}
		} else if (strcmp(inputf, "deltask") == 0) {
			if (strcmp(group, "none") != 0) {
				if (inputb && inputb[0] != '\0') {
					char tmp[512];
					char tmpt[512];
					char tmpw[512];
					char tmpd[512];
					snprintf(tmpt, sizeof(tmpt), "%s/.task/%s/todo/%s.md", home, group, inputb);
					snprintf(tmpw, sizeof(tmpw), "%s/.task/%s/inwork/%s.md", home, group, inputb);
					snprintf(tmpd, sizeof(tmpd), "%s/.task/%s/done/%s.md", home, group, inputb);
					if (access(tmpt, F_OK) == 0 || access(tmpw, F_OK) == 0 || access(tmpd, F_OK) == 0) {
						snprintf(tmp, sizeof(tmp), "rm -f '%s/.task/%s/'*'/%s.md'", home, group, inputb);
						system(tmp);
					} else {
						printf("Task %s dosen\'t exist\n", inputb);
					}
				} else {
					printf("%s requires an argument\n", inputf);
				}
			} else {
				printf("No group selected\n");
			}
		} else if (strcmp(inputf, "modtask") == 0) {
			if (strcmp(group, "none") != 0) {
				if (inputb && inputb[0] != '\0') {
					char tmp[512];
					char tmpt[512];
					char tmpw[512];
					char tmpd[512];
					snprintf(tmpt, sizeof(tmpt), "%s/.task/%s/todo/%s.md", home, group, inputb);
					snprintf(tmpw, sizeof(tmpw), "%s/.task/%s/inwork/%s.md", home, group, inputb);
					snprintf(tmpd, sizeof(tmpd), "%s/.task/%s/done/%s.md", home, group, inputb);
					if (access(tmpt, F_OK) == 0 || access(tmpw, F_OK) == 0 || access(tmpd, F_OK) == 0) {
						snprintf(tmp, sizeof(tmp), "%s '%s/.task/%s/'*'/%s.md'", edit->valuestring, home, group, inputb);
						system(tmp);
					} else {
						printf("Task %s dosen\'t exist\n", inputb);
					}
				} else {
					printf("%s requires an argument\n", inputf);
				}
			} else {
				printf("No group selected\n");
			}			
		} else if (strcmp(inputf, "infotask") == 0) {
			if (strcmp(group, "none") != 0) {
				if (inputb && inputb[0] != '\0') {
					char tmp[512];
					char tmpt[512];
					char tmpw[512];
					char tmpd[512];
					snprintf(tmpt, sizeof(tmpt), "%s/.task/%s/todo/%s.md", home, group, inputb);
					snprintf(tmpw, sizeof(tmpw), "%s/.task/%s/inwork/%s.md", home, group, inputb);
					snprintf(tmpd, sizeof(tmpd), "%s/.task/%s/done/%s.md", home, group, inputb);
					if (access(tmpt, F_OK) == 0 || access(tmpw, F_OK) == 0 || access(tmpd, F_OK) == 0) {
						snprintf(tmp, sizeof(tmp), "%s '%s/.task/%s/'*'/%s.md'", cat->valuestring, home, group, inputb);
						system(tmp);
					} else {
						printf("Task %s dosen\'t exist\n", inputb);
					}
				} else {
					printf("%s requires an argument\n", inputf);
				}
			} else {
				printf("No group selected\n");
			}	
		} else if (strcmp(inputf, "donetask") == 0) {
			if (strcmp(group, "none") != 0) {
				if (inputb && inputb[0] != '\0') {
					char tmp[512];
					char tmpt[512];
					char tmpw[512];
					char tmpd[512];
					snprintf(tmpt, sizeof(tmpt), "%s/.task/%s/todo/%s.md", home, group, inputb);
					snprintf(tmpw, sizeof(tmpw), "%s/.task/%s/inwork/%s.md", home, group, inputb);
					snprintf(tmpd, sizeof(tmpd), "%s/.task/%s/done/%s.md", home, group, inputb);
					if (access(tmpt, F_OK) == 0 || access(tmpw, F_OK) == 0 || access(tmpd, F_OK) == 0) {
						snprintf(tmp, sizeof(tmp), "mv '%s/.task/%s/'*'/%s.md' '%s/.task/%s/done/%s.md'", home, group, inputb, home, group, inputb);
						system(tmp);
					} else {
						printf("Task %s dosen\'t exist\n", inputb);
					}
				} else {
					printf("%s requires an argument\n", inputf);
				}
			} else {
				printf("No group selected\n");
			}	
		} else if (strcmp(inputf, "inworktask") == 0) {
			if (strcmp(group, "none") != 0) {
				if (inputb && inputb[0] != '\0') {
					char tmp[512];
					char tmpt[512];
					char tmpw[512];
					char tmpd[512];
					snprintf(tmpt, sizeof(tmpt), "%s/.task/%s/todo/%s.md", home, group, inputb);
					snprintf(tmpw, sizeof(tmpw), "%s/.task/%s/inwork/%s.md", home, group, inputb);
					snprintf(tmpd, sizeof(tmpd), "%s/.task/%s/done/%s.md", home, group, inputb);
					if (access(tmpt, F_OK) == 0 || access(tmpw, F_OK) == 0 || access(tmpd, F_OK) == 0) {
						snprintf(tmp, sizeof(tmp), "mv '%s/.task/%s/'*'/%s.md' '%s/.task/%s/inwork/%s.md'", home, group, inputb, home, group, inputb);
						system(tmp);
					} else {
						printf("Task %s dosen\'t exist\n", inputb);
					}
				} else {
					printf("%s requires an argument\n", inputf);
				}
			} else {
				printf("No group selected\n");
			}	
		} else if (strcmp(inputf, "todotask") == 0) {
			if (strcmp(group, "none") != 0) {
				if (inputb && inputb[0] != '\0') {
					char tmp[512];
					char tmpt[512];
					char tmpw[512];
					char tmpd[512];
					snprintf(tmpt, sizeof(tmpt), "%s/.task/%s/todo/%s.md", home, group, inputb);
					snprintf(tmpw, sizeof(tmpw), "%s/.task/%s/inwork/%s.md", home, group, inputb);
					snprintf(tmpd, sizeof(tmpd), "%s/.task/%s/done/%s.md", home, group, inputb);
					if (access(tmpt, F_OK) == 0 || access(tmpw, F_OK) == 0 || access(tmpd, F_OK) == 0) {
						snprintf(tmp, sizeof(tmp), "mv '%s/.task/%s/'*'/%s.md' '%s/.task/%s/todo/%s.md'", home, group, inputb, home, group, inputb);
						system(tmp);
					} else {
						printf("Task %s dosen\'t exist\n", inputb);
					}
				} else {
					printf("%s requires an argument\n", inputf);
				}
			} else {
				printf("No group selected\n");
			}	
		} else if (strcmp(inputf, "lsgrp") == 0) {
			system("find ~/.task -mindepth 1 -maxdepth 1 -type d -printf '\%f\\n'");
		} else if (strcmp(inputf, "lstask") == 0) {
			if (strcmp(group, "none") != 0) {
					char todo[1024]; 
					snprintf(todo, sizeof(todo), "find '%s/.task/%s/todo' -maxdepth 1 -type f -name '*.md' -printf '%%f\\n' | sed 's/\\.md$//'", home, group);
					printf(" To Do: \n");
					system(todo);
					char inwork[1024];
					snprintf(inwork, sizeof(inwork), "find '%s/.task/%s/inwork' -maxdepth 1 -type f -name '*.md' -printf '%%f\\n' | sed 's/\\.md$//'", home, group);
					printf(" In Work: \n");
					system(inwork);
					char done[1024];
					snprintf(done, sizeof(done), "find '%s/.task/%s/done' -maxdepth 1 -type f -name '*.md' -printf '%%f\\n' | sed 's/\\.md$//'", home, group);
					printf(" Done: \n");
					system(done);

			} else {
				printf("No group selected\n");
			}
		} else if (strcmp(inputf, "help") == 0) {
			printf("exit - to exit ctl\naddgrp foo - to add new group named foo\ndelgrp foo - to delete group foo\nchgrp foo - changes working group to foo\naddtask foo - to add new task named foo in working group\ndeltask foo - to delete task foo in working group\ninfotask foo - shows information about task foo in working group\nmodtask foo - edits informations about task foo in working group\ndonetask foo - flags task foo as done\ninworktask foo - flags task foo as in work\ntodotask foo - flags task foo as un done\nlsgrp - list all groups\nlstask - list all task in working group\nconfig - alows to configure programs to open and edit tasks\nhelp - shows this menu\n");
		} else if (strcmp(inputf, "config") == 0) {
			char buffere[512];
			
			printf("What command do you wanna use to edit markdown task files. Now: \"%s\". Leave empty to not change\n", edit->valuestring);
			printf(">>> ");
			if (!fgets(buffere, sizeof(buffere), stdin)) {
				printf("EOF\n");
				break;
			}
			buffere[strcspn(buffere, "\n")] = 0;

			if (buffere[0] != '\0') {
				cJSON_SetValuestring(edit, buffere);
			}

			char bufferc[512];

			printf("What command do you wanna use to cat markdown task files. Now: \"%s\". Leave empty to not change\n", cat->valuestring);
			printf(">>> ");
			if (!fgets(bufferc, sizeof(bufferc), stdin)) {
				printf("EOF\n");
				break;
			}
			bufferc[strcspn(bufferc, "\n")] = 0;

			if (bufferc[0] != '\0') {
				cJSON_SetValuestring(cat, bufferc);
			}
			
			char *out = cJSON_Print(json);

			FILE *fw = fopen(pathconfig, "w");
			fputs(out, fw);
			fclose(fw);

			free(out);
		} else if (strcmp(inputf, "") != 0) {
			printf("Unknown command %s, type in help for help\n", inputf);
		}
	}
	return 0;
}
