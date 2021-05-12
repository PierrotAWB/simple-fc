#include "main.h"
#include "defs.h"
#include "return_codes.h"
#include <stdio.h>
#include <stdlib.h>

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

	printf("Welcome to learn mode.\n\n"

		   "Each round, you'll be presented with the front of a card, whose back\n"
		   "face can be revealed by inputting any sequence of characters\n"
		   "and pressing enter (the exception is 'q', which quits learn mode).\n\n"

		   "Note that the input is whitespace and case sensitive.\n"
		   "If your input matches the back face exactly, you will be told so.\n"
		   "Otherwise, you'll be given what the back face reads.\n\n"

		   "There is no time limit, and no data will be recorded.\n\n");

	int idx;
	size_t sz = MAX_CMD_LENGTH;
	char *input = malloc(sz);

	while (1) {
		idx = rand() % numCards;
		printf("---\n%s\n---\n", cards[idx]->front);

		getline(&input, &sz, stdin);
		STRIP(input);

		if (!strncmp(input, "q", 1)) {
			printf("\nExiting learn mode.\n\n");
			break;
		} else if (!strcmp(input, cards[idx]->back)) {
			printf("\nCorrect!\n\n");
		} else {
			printf("\nThe back of this card reads:\n\n%s\n\n", cards[idx]->back);
		}
	}

	free(input);

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

