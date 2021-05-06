#include "main.h"
#include <stdio.h>

int
cmd_quit(int nargs, char **args)
{
	(void) nargs;
	(void) args;
	shouldRun = 0;
	printf("Quitting. Bye!\n");
	return 0;
}

int
cmd_help(int nargs, char **args)
{
  	/* TODO: More detailed help; emulate gparted help */
	(void) nargs;
	(void) args;
	printf("This is the help menu.\n");
	return 0;
}

int
cmd_load(int nargs, char **args)
{
	(void) nargs;
	(void) args;
	printf("Do load.\n");
	return 0;
}

int
cmd_learn(int nargs, char **args)
{
	(void) nargs;
	(void) args;
	printf("Enter learn mode.\n");
	return 0;
}

int
cmd_practice(int nargs, char **args)
{
	(void) nargs;
	(void) args;
	printf("Enter practice mode.\n");
	return 0;
}

int
cmd_test(int nargs, char **args)
{
	(void) nargs;
	(void) args;
	printf("Enter test mode.\n");
	return 0;
}

