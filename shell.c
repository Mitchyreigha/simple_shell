#include "main.h"

/**
 * execute_command - Executes the command using execve
 * @command: The command to execute
 * @args: The arguments for the command
 */
void execute_command(char *command, char **args)
{
	if (execve(command, args, environ) == -1)
	{
		perror("execve error");
		exit(EXIT_FAILURE);
	}
}

/**
 * prog_exec - It executes the command read from read_input
 * @args: Pointer to the command tokens read by read_input
 */
void prog_exec(char **args)
{
	pid_t pid;
	int status;
	char *command = args[0];
	char *full_path = NULL;

	pid = fork();
	if (pid < 0)
	{
		perror("Fork Failed");
	}
	else if (pid == 0)
	{
		if (strchr(command, '/') != NULL)
		{
			/* Execute the command directly if it's a full path */
			execute_command(command, args);
		}
		else
		{
			/* Try to find the full path for the command */
			full_path = get_full_path(command);
			if (full_path == NULL)
			{
				fprintf(stderr, "%s: command not found\n", command);
				exit(EXIT_FAILURE);
			}
			execute_command(full_path, args);
			free(full_path);
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
 * handle_command - Handle the execution of commands (including built-ins)
 * @input: User input
 * @args: Command and arguments
 */
void handle_command(char *input, char **args)
{
	int argc = read_input(input, args);

	if (argc > 0 && strcmp(args[0], "exit") == 0)
	{
		/* Handle the 'exit' command */
		exit(EXIT_SUCCESS);
	}
	else if (argc > 0 && strcmp(args[0], "cd") == 0)
	{
		/*  Handle the 'cd' command */
		if (argc < 2)
			fprintf(stderr, "cd: expected argument\n");
		else if (chdir(args[1]) != 0)
			perror("cd error");
	}
	else if ((argc > 0) && (strcmp(args[0], "list") == 0
				|| strcmp(args[0], "display") == 0))
	{
		/* Handle the 'list' and 'display' commands */
		if (argc < 2)
			fprintf(stderr, "%s: expected argument\n", args[0]);
		else if (strcmp(args[0], "list") == 0)
			list_directory_contents(args[1]);
		else if (strcmp(args[0], "display") == 0)
			display_file_content(args[1]);
	}
	else if (argc > 0 && strcmp(args[0], "env") == 0)
	{
		/* Handle the 'env' command */
		print_environment();
	}
	else
	{
		/* Execute other commands using prog_exec */
		prog_exec(args);
	}
}
