#include "main.h"
/**
 * prog_exec - It executes the command read from read_input using execve
 * @args: pointer to the command token read by read_input
 */
void prog_exec(char **args)
{
	pid_t pid;
	int status;
	char *full_path;


	pid = fork();
	if (pid < 0)
		perror("Fork Failed");

	else if (pid == 0)
	{
		full_path = get_full_path(args[0]);
		if (full_path == NULL)
		{
			fprintf(stderr, "%s: command not found\n", args[0]);
			exit(EXIT_FAILURE);
		}
		if (execve(full_path, args, environ) == -1)
		{
			perror("execve error");
			free(full_path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		while (1)
		{
			waitpid(pid, &status, WUNTRACED);
			if (WIFEXITED(status) || WIFSIGNALED(status))
				break;
		}
	}
}

/**
 * get_full_path - locates the command to be executed
 * @command: the command to be executed
 * Return: The command path to be executed
 */
char *get_full_path(char *command)
{
	char *path_env, *full_path, *token;
	int access_result;

	path_env = getenv("PATH");
	if (path_env == NULL)
		return (NULL);
	token = strtok(path_env, ":");
	while (token)
	{
		full_path = (char *)malloc(strlen(token) + strlen(command) + 2);
		if (full_path == NULL)
		{
			perror("malloc error");
			exit(EXIT_FAILURE);
		}

		strcpy(full_path, token);
		strcat(full_path, "/");
		strcat(full_path, command);

		access_result = access(full_path, X_OK);
		if (access_result == 0)
			return (full_path);
		free(full_path);

		token = strtok(NULL, ":");
	}
	return (NULL);
}

/**
 * read_input - It reads and isolate each input to the shell into strings
 * of commands and arguments.
 * @input: Commands given to the shell
 * @args: argument string given to the shell
 * Return: Number of strings passed to the shell
 */

int read_input(char *input, char **args)
{
	int i = 0;
	char *token = NULL;
	const char *delim = " \t\n\r\a";

	token = strtok(input, delim);
	while (token)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, delim);
	}
	args[i] = NULL;
	return (i);
}

/**
 * run_shell - creates the shell and execute commands
 * Return: EXIT_SUCCESS;
 */

int run_shell(void)
{
	char *input = NULL;
	char *args[MAX_ARGS];
	int argc;
	size_t input_size = 0;

	while (1)
	{
		printf(PROMPT);
		if (getline(&input, &input_size, stdin) == -1)
		{
			printf("\n");
			break;
		}
		argc = read_input(input, args);
		if (argc > 0 && strcmp(args[0], "exit") == 0)
			break;
		else if (argc > 0 && strcmp(args[0], "cd") == 0)
		{
			if (argc < 2)
				fprintf(stderr, "cd: expected argument\n");
			else if (chdir(args[1]) != 0)
				perror("cd error");
		}
		else if ((argc > 0) && (strcmp(args[0], "list") == 0
					|| strcmp(args[0], "display") == 0))
		{
			if (argc < 2)
				fprintf(stderr, "%s: expected argument\n", args[0]);
			else if (strcmp(args[0], "list") == 0)
				list_directory_contents(args[1]);
			else if (strcmp(args[0],  "display") == 0)
				display_file_content(args[1]);
		}
		else
			prog_exec(args);
	}
	free(input);
	return (0);
}
/**
 * main - the beginning of the program execution
 * Return: 0 Always Success
 */
int main(void)
{
	run_shell();
	return (EXIT_SUCCESS);
}
