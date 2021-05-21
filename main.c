#include "cmd.h"
#include "defs.h"
#include "main.h"
#include "return_codes.h"
#include "time.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static int dispatch(char *cmd);
static FILE *openCardFile(char *cardFilePath);
static void run(void);
static void setup(char *);
static void teardown(void);

static size_t cardsCapacity = 10;
static char *simpleFcHome = NULL;

/* Globals */
struct Card **cards;
size_t numCards = 0;
bool shouldRun = 1;

char *
readFace(FILE *cardFile, const char *sentinel)
{
	// TODO: Error handling. What happens if we have an unpaired <back></back> pair?
	// What if we have an unpaired <front> or <back>?

	char line[MAX_CARD_FACE_CHARS], text[MAX_CARD_FACE_CHARS];
	int tmp = 0;
	text[0] = '\0';

	// TODO: replace fgets with getline()
	while (fgets(line, sizeof(line), cardFile)) {
		if (IS_PREFIX(sentinel, line)) break;
		tmp = 0;
		while (isspace((unsigned char) *(line + tmp++))) { continue; }
		strncat(text, line + tmp - 1, (int) strlen(line) - tmp + 1);
	}

	if (strlen(text) > 0) {
		STRIP(text);
		return strdup(text);
	} else 
		return NULL;
}

FILE *
openCardFile(char *cardFilePath)
{
	STRIP(cardFilePath);
	if (cardFilePath[0] == '/') {
		/* Path is absolute; don't prepend anything. */
		return fopen(cardFilePath, "r");
	}

	char *path = malloc(MAX_FILEPATH_LENGTH * sizeof(char *));
	strcpy(path, simpleFcHome);
	strcat(path, cardFilePath);
	return fopen(path, "r");
}

int
populateCards(char *cardFilePath)
{
	FILE *cardFile = openCardFile(cardFilePath);

	if (!cardFile) {
		fprintf(stderr, "Unable to open file \"%s\".\n\n", cardFilePath);
		return 0;
	}

	int cardsLoaded = 0;
	char line[MAX_CARD_FACE_CHARS];

	// TODO: replace fgets() with getline()
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
				if (numCards >= cardsCapacity) {
					struct Card **tmp = realloc(cards, sizeof *tmp * (2*cardsCapacity));
					if (tmp)
					{
						cards = tmp;
						cardsCapacity *= 2;
					}
				}
				cards[numCards++] = currCard;
				++cardsLoaded;
			}
		}
	}

	return cardsLoaded;
}

void
setup(char *catalogueName)
{
	char line[MAX_FILEPATH_LENGTH], cataloguePath[MAX_FILEPATH_LENGTH];

	cards = malloc(cardsCapacity * sizeof(struct Card *));

	char *dataHome = getenv("XDG_DATA_HOME");
	simpleFcHome = strcat(dataHome, "/simplefc/");
	strcpy(cataloguePath, simpleFcHome);

	if (catalogueName == NULL)
		strcat(cataloguePath, "catalogue");
	else 
		strcat(cataloguePath, catalogueName);

	FILE *catalogue = fopen(cataloguePath, "r");

	if (catalogue == NULL) {
		printf("Could not find catalogue file at $XDG_DATA_HOME/simplefc/catalogue.\n"
			   "Please add catalogue, or load cards manually with the 'L' command.\n\n");
	} else {
		while (fgets(line, sizeof(line), catalogue)) { populateCards(line); }
		fclose(catalogue);
	}

	/* Set seed for rand(). */
	srand(time(NULL));
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
			word = strtok_r(NULL, " \t", &context)) {
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
run(void)
{
	printf("%s\n", STARTUP_MESSAGE);
	printf("You currently have %ld card(s) loaded.\n\n", numCards);

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
				fprintf(stderr, "Unknown command: \"%s\".\n\n", cmd);
				break;
			case E_TOO_MANY_ARGS:
				fprintf(stderr, "Too many arguments.\n\n");
		}
	}

	free(cmd);
}

int
main(int argc, char **argv)
{
	if (argc == 2)
		setup(argv[1]);
	else if (argc == 1) 
		setup(NULL);
	run();
	teardown();
	return SUCCESS;
}
