#include "shell.h"
/**
 * _error - Handles errors and print error messages
 * @message: pointer to the message to be printed
 * Return: void
 */
void _error(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

/**
 * execute_command - Execute a command
 * @command: pointer to the command to be executed
 * Return: void
 */
void execute_command(char *command)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
		_error("fork");

	if (child_pid == 0)
	{
		char **args = malloc(2 * sizeof(char *));

		if (args == NULL)
			_error("malloc");
		args[0] = command;
		args[1] = NULL;

		execve(command, args, NULL);
		_error(command);
		exit(EXIT_FAILURE);
	}
	else
		wait(&status);
}

/**
 * main - Beginning of the code
 * Return: 0 Always Successful
 */
int main(void)
{
	char *line = NULL, *prompt = "#cisfun$: ";
	size_t len = 0;
	ssize_t nread;

	while (1)
	{
		write(STDOUT_FILENO, prompt, strlen(prompt));
		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (feof(stdin))
			{
				write(STDOUT_FILENO, "\n", 1);
				break;
			}
			_error("getline");
		}

		if (nread > 1)
		{
			line[nread - 1] = '\0';
			execute_command(line);
		}
	}
	free(line);
	exit(EXIT_SUCCESS);
}
