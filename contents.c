#include "main.h"

/**
 * list_directory_contents - Function to list directory contents
 * @dir_path: pointer to the directory location
 * Return: Void
 */
void list_directory_contents(const char *dir_path)
{
	struct dirent *entry;
	DIR *dp = opendir(dir_path);

	if (dp == NULL)
	{
		perror("opendir");
		return;
	}

	printf("Contents of directory %s:\n", dir_path);
	while ((entry = readdir(dp)))
	{
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			printf("%s\n", entry->d_name);
		}
	}

	closedir(dp);
}

/**
 * display_file_content - Function to display file content
 * @file_path: pointer to the location of file path
 * Return: Void
 */
void display_file_content(const char *file_path)
{
	char buffer[1024];
	FILE *file = fopen(file_path, "r");

	if (file == NULL)
	{
		perror("fopen");
		return;
	}

	printf("Contents of file %s:\n", file_path);
	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		printf("%s", buffer);
	}

	fclose(file);
}
