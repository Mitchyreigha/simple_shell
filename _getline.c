#include "main.h"

/**
 * _getline - Custom implementation of getline using dynamic memory allocation
 * @lineptr: Pointer to the buffer to store the line
 * @n: Pointer to the size of the buffer
 * @stream: File stream to read from (usually stdin)
 * Return: Number of characters read (including the newline character)
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	ssize_t bytes_read = 0;
	size_t bufsize = 0;
	int c;

	if (!lineptr || !n)
		return (-1);

	if (!*lineptr || !*n)
	{
		bufsize = 1024;
		*lineptr = malloc(bufsize);

		if (!*lineptr)
			return (-1);

		*n = bufsize;
	}

	while ((c = fgetc(stream)) != EOF && ((*lineptr)[bytes_read++] = c))
	{
		if (bytes_read + 1 >= (ssize_t)(*n))
		{
			bufsize *= 2;
			*lineptr = realloc(*lineptr, bufsize);

			if (!*lineptr)
				return (-1);

			*n = bufsize;
		}

		if (c == '\n')
			break;
	}

	return ((bytes_read > 0) ? bytes_read : -1);
}
