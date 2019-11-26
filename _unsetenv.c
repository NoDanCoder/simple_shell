#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

/**
 * _copydoublepDel - copies an array of strings (double pointer)
 *
 * @p: double pointer to copy
 * @new_size: size of copy
 * @jump: value that must be skipped in copy
 *
 * Return: Pointer malloec
 */
char **_copydoublepDel(char **p, int new_size, int jump)
{
	char **copy;
	int i, j, csize;

	csize = new_size;
	copy = malloc(sizeof(char *) * (csize + 1));
	if (copy == 0)
		return (0);

	for (i = 0, j = 0; j < csize; i++, j++)
	{
		if (i == jump)
			i++;
		copy[j] = _strdup(p[i]);
		if (copy[j] == 0)
		{
			j--;
			for (; j >= 0; j--)
				free(copy[j]);
			free(copy);
			return (0);
		}
	}

	copy[new_size] = '\0';

	return (copy);
}

/**
 * _unsetenv - unsets an enviromental variable
 *
 * @env: array of env variables
 * @variable: env variable to unset
 * @shpack: struct with shell info
 *
 * Return: 0 on success, -1 on error
 */
char **_unsetenv(char **env, char *variable, hshpack *shpack)
{
	int i, j, check, l = 0, lenv = 0;
	char **copy;

	if (_strlen(variable) == 0 || variable == 0)
		return (_error(3, shpack, 1), NULL);

	l = _strlen(variable), lenv = _strlendp(env);
	for (i = 0; env[i] != 0; i++)
	{
		for (check = 0, j = 0; j < l && env[i][j] != 0; j++)
		{
			if (variable[j] == '=')
				return (_error(3, shpack, 2), NULL);
			if (env[i][j] == variable[j])
				check++;
		}
		if (check == l && env[i][check] == '=')
		{
			/* Found env to erase */
			copy = _copydoublepDel(env, lenv - 1, i), free_doubpoint(env);
			if (copy == 0)
				return (_error(7, shpack, 1), NULL);
			env = copy;
			return (env);
		}
	}
	return (env);
}
