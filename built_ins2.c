#include <limits.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "shell.h"

/**
 * _cd_cmd - built in command cd
 * @shpack: struct containing shell info
 *
 * Return: 1 if succesful, -1 if fail
 */
ssize_t _cd_cmd(hshpack *shpack)
{
	char *currdir, *dir, **newenv,  *home;
	int exit = 1, check = 1;

	currdir = getcwd(NULL, 100);
	if (shpack->options[1])
	{
		if (!_strcmp(shpack->options[1], "-"))
		{
			dir = shpack->prevcd;
			write(1, dir, _strlen(dir)), write(1, "\n", 1);
		}
		else
			dir = shpack->options[1];
	}
	else
	{
		home = _getenv("HOME", *(shpack->envCpy));
		if (home)
			dir = home + 5;
		else
			dir = NULL;
	}
	if (dir)
		check = chdir(dir);
	if (check != 0)
	{
		_error(4, shpack, 2);
		exit = -1;
	}
	else
	{
		newenv = _setenv(*(shpack->envCpy), "PWD", dir, shpack);
		free(shpack->prevcd), shpack->prevcd = _strdup(currdir);
		*(shpack->envCpy) = newenv;
		newenv = _setenv(*(shpack->envCpy), "OLDPWD", currdir, shpack);
		*(shpack->envCpy) = newenv;
	}
	free(shpack->options), free(currdir);
	return (exit);
}
