#include "main.h"
/**
 * main - the beginning of the program execution
 * Return: 0 Always Success
 */
int main(void)
{
	run_shell();
	return (EXIT_SUCCESS);
}

/**
 * print_environment - Print the current environment variables
 */
void print_environment(void)
{
	char **env = environ;

	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}

/**
 * run_shell - Creates the shell and executes commands
 * Return: EXIT_SUCCESS;
 */
int run_shell(void)
{
	char *input = NULL;
	char *args[MAX_ARGS];
	ssize_t bytes_read;
	size_t n = 0;

	while (1)
	{
		printf(PROMPT);
		bytes_read = _getline(&input, &n, stdin);

		if (bytes_read <= 0)
		{
			printf("\n");
			break;
		}
		if (bytes_read == 1)
			continue;

		handle_command(input, args);

		free(input);
	}

	return (EXIT_SUCCESS);
}
