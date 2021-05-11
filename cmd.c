#include "main.h"
#include "return_codes.h"
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
	(void) args;

	if (nargs == 1) {
		printf("\n[L]		Load new cards.\n"
				 "[l]		Enter learn mode.\n"
				 "[p]		Enter practice mode.\n"
				 "[t]		Enter test mode.\n"
				 "[h]		Show this menu.\n"
				 "[q]		Quit the program.\n\n");
	} else if (nargs == 2) {
		/* TODO: Give more detailed help; use case for each command. */
		printf("Getting help for %s.\n\n", args[1]);
	} else {
		return E_TOO_MANY_ARGS; /* Bad number of arguments */
	}
	return 0;
}

int
cmd_load(int nargs, char **args)
{
	if (nargs == 1) {
		printf("Expected path to card-file.\n\n");
	} else {
		int cardsLoaded;
		for (int i = 1; i < nargs; ++i) {
			cardsLoaded = populateCards(args[i]);
			printf("Successfully loaded %d cards from %s.\n", cardsLoaded, args[i]);
		}
		printf("\n");
	}
	return 0;
}

int
cmd_learn(int nargs, char **args)
{
	(void) nargs;
	(void) args;
	printf("Entering learn mode.\n");
	return 0;
}

int
cmd_practice(int nargs, char **args)
{
	(void) nargs;
	(void) args;
	printf("Entering practice mode.\n");
	return 0;
}

int
cmd_test(int nargs, char **args)
{
	(void) nargs;
	(void) args;
	printf("Entering test mode.\n");
	return 0;
}

