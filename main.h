#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64
#define PROMPT "CisFun:$ "

extern char **environ;

int read_input(char *input, char **args);
void prog_exec(char **args);
int run_shell(void);
char *get_full_path(char *command);
void display_file_content(const char *file_path);
void list_directory_contents(const char *dir_path);

#endif
