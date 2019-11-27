#include <limits.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "shell.h"

/**
 * auxcd2 - auxiliary function of cd built in
 * @shpack: struct containing shell info
 * @currdir: current directory
 *
 * Return: pointer to HOME or NULL if fail
 */
char *auxcd2(hshpack *shpack, char *currdir)
{
	char *home, *dir;

	home = _getenv("HOME", *(shpack->envCpy));
	if (home)
		dir = home + 5;
	else
	{
		write(2, "HOME env variable not set\n", 26);
		free(shpack->options), free(currdir);
		return (shpack->exitnum[0] = 2, NULL);
	}

	return (dir);
}

/**
 * auxcd - auxiliary function of cd built in
 * @shpack: struct containing shell info
 * @currdir: the current directory
 *
 * Return: Pointer to dir or NULL if fail
 */
char *auxcd(hshpack *shpack, char *currdir)
{
	char *oldpwd2 = NULL, *oldpwd = NULL, *dir = NULL;

	oldpwd2 = _strdup(_getenv("OLDPWD", *(shpack->envCpy)));
	if (oldpwd2)
		oldpwd = _strdup(oldpwd2 + 7), free(oldpwd2);
	if (!oldpwd2)
	{
		write(2, "OLDPWD env variable not set\n", 28);
		free(oldpwd), free(shpack->options), free(currdir);
		return (shpack->exitnum[0] = 2, NULL);
	}

	dir = oldpwd;

	return (dir);
}

/**
 * _cd_cmd - built in command cd
 * @shpack: struct containing shell info
 *
 * Return: 1 if succesful, -1 if fail
 */
ssize_t _cd_cmd(hshpack *shpack)
{
	char *currdir = NULL, *dir = NULL, **newenv, *oldpwd = NULL;
	int exit = 1, check = 1, checkminus = 0;

	currdir = getcwd(NULL, 100);
	if (shpack->options[1])
	{
		if (!_strcmp(shpack->options[1], "-"))
		{
			dir = auxcd(shpack, currdir);
			if (!dir)
				return (-1);
			checkminus = 1;
		}
		else
			dir = shpack->options[1];
	}
	else
	{
		dir = auxcd2(shpack, currdir);
		if (!dir)
			return (-1);
	}
	if (dir)
		check = chdir(dir);
	if (check == 0 && checkminus == 1)
		write(1, dir, _strlen(dir)), write(1, "\n", 1);
	if (check != 0)
		_error(4, shpack, 2), exit = -1;
	else
	{
		newenv = _setenv(*(shpack->envCpy), "PWD", dir, shpack);
		*(shpack->envCpy) = newenv;
		newenv = _setenv(*(shpack->envCpy), "OLDPWD", currdir, shpack);
		*(shpack->envCpy) = newenv;
	}
	free(shpack->options), free(currdir), free(oldpwd);
	if (checkminus == 1)
		free(dir);
	return (exit);
}
