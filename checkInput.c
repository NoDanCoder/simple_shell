#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "shell.h"

/**
 * checkInput - checks for input in after shell prompt
 * @ac: count of main arguments
 * @av: main arguments
 * @bufsize: size of buffer in prompt
 * @buffer: buffer in prompt
 * @shpack: struct of shell info
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
char **checkInput(int ac, char **av, size_t *bufsize,
		   char **buffer, hshpack *shpack)
{
	ssize_t characters;
	char **command;
	int exitnum;

	if (ac == 1)
	{
		if (isatty(STDIN_FILENO))
			write(1, "$ ", 2);
		characters = getline(buffer, bufsize, stdin);

		if (characters == -1)
		{
			exitnum = shpack->exitnum[0];
			free(*buffer);
			free(shpack->prevcd);
			free_doubpoint(*(shpack->envCpy));
			free(shpack);
			if (isatty(STDIN_FILENO))
				write(1, "\n", 1);
			exit(exitnum);
		}
		else if (!characters || **buffer == '\n')
			return (NULL);
		*buffer = _strtok(*buffer, "#");
		command = getParameters(*buffer, shpack);
	}
	else
	{
		command = malloc(sizeof(char *) * (ac - 1));
		if (!command)
		{
			_error(7, shpack, 1);
			return (NULL);
		}

		command[ac - 1] = '\0';
		while (ac--)
			command[ac - 1] = av[ac];
	}
	return (command);
}