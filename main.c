#include "cmd.h"
#include "defs.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* Macros */
#define IS_PREFIX(pref, str) !strncmp((str), (pref), sizeof((pref))-1)
#define STRIP(s) (s)[strcspn((s), "\n")] = 0

static int dispatch(char *cmd);
static void populateCards(char *filename);
static void run(void);
static void setup(void);
static void teardown(void);

static struct Card **cards;
static size_t cardsCapacity = 10;
static size_t numCards = 0;

bool shouldRun = 1;

char *
readFace(FILE *cardFile, const char *sentinel)
{
	// TODO: Error handling. What happens if we have an unpaired <back></back> pair?
	// What if we have an unpaired <front> or <back>?

	char line[MAX_CARD_FACE_CHARS], text[MAX_CARD_FACE_CHARS];
	text[0] = '\0';

	while (fgets(line, sizeof(line), cardFile)) {
		if (IS_PREFIX(sentinel, line)) break;
		// TODO: Strip leading whitespace.
		strncat(text, line, (int) strlen(line));
	}

	return (strlen(text) > 0) ? strdup(text) : NULL;
}

void
populateCards(char *filename)
{
	char path[MAX_FILEPATH_LENGTH];
	STRIP(filename);

	strcpy(path, "example/");
	strcat(path, filename);
	FILE *cardFile = fopen(path, "r");

	if (!cardFile) {
		fprintf(stderr, "Unable to open file \"%s\"\n.", path);
		return;
	}

	char line[MAX_CARD_FACE_CHARS];

	while (fgets(line, sizeof(line), cardFile)) {
		if (IS_PREFIX(FRONT_TAG_OPEN, line)) {
			struct Card *currCard = malloc(sizeof(struct Card *));
			currCard->front = readFace(cardFile, FRONT_TAG_CLOSE);

			while (fgets(line, sizeof(line), cardFile)
					&& IS_PREFIX(BACK_TAG_OPEN, line)) {
				// Consume empty space and BACK_TAG_OPEN.
				continue;
			}

			currCard->back = readFace(cardFile, BACK_TAG_CLOSE);

			if (currCard->front && currCard->back) {
				if (numCards > cardsCapacity) {
					cards = (struct Card **) realloc(cards,
							2 * cardsCapacity * sizeof(struct Card *));
				}
				cards[numCards++] = currCard;
			}
		}
	}
}

void
setup(void)
{
	/* Malloc card */
	cards = malloc(cardsCapacity * sizeof(struct Card *));

	/* Read from file; path temporarily hard-coded */
	FILE *catalogue = fopen("example/catalogue.txt", "r");;

	char line[MAX_FILEPATH_LENGTH];

	while (fgets(line, sizeof(line), catalogue)) {
		populateCards(line);
	}
	fclose(catalogue);
}

void
teardown(void)
{
	for (int i = 0; i < (int) numCards; ++i) free(cards[i]);
	free(cards);

	return;
}


static
int
dispatch(char *cmd)
{
	char *args[MAX_CMD_ARGS];
	char *word, *context;
	int nargs = 0;

	for (word = strtok_r(cmd, " \t", &context);
			word != NULL;
			word = strtok_r(NULL, "\t", &context)) {
		if (nargs >= MAX_CMD_ARGS)
			return E_TOO_MANY_ARGS;
		args[nargs++] = word;
	}

	if (nargs == 0) return SUCCESS;

	for (int i = 0; command_table[i].name; i++) {
		if (*command_table[i].name &&
				!strcmp(args[0], command_table[i].name)) {
			return command_table[i].func(nargs, args);
		}
	}

	return E_UNKNOWN_CMD;
}

void
run(void) /* TODO: Accept arguments */
{
	printf("%s\n", STARTUP_MESSAGE);
	printf("You currently have %ld cards loaded.\n\n", numCards);

	char *cmd; 
	size_t sz = MAX_CMD_LENGTH;
	cmd = (char *) malloc(sz);

	shouldRun = 1;

	while (shouldRun) {
		printf("Enter a command ['?' or 'h' for menu]: ");
		
		getline(&cmd, &sz, stdin);
		STRIP(cmd);

		int result = dispatch(cmd);

		switch (result) {
			case E_UNKNOWN_CMD:
				fprintf(stderr, "Unknown command: \"%s\".\n", cmd);
				break;
			case E_TOO_MANY_ARGS:
				fprintf(stderr, "Too many arguments.");
		}
	}

	free(cmd);
	
//  			case 'L':
//  				/* Load more cards */
//  			case 'l':
//  				/* Learning mode */
//  			case 'p':
//  				/* Practice mode */
//  			case 't':
//  				/* Test mode */
//  				break;
//  			default:
//  				printf("Unrecognized option.\n");
//  		}
//  	}
}

int
main(void)
{
	setup();
	run();
	teardown();
	return SUCCESS;
}
