#include "defs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* Macros */
#define IS_PREFIX(pref, str) !strncmp((str), (pref), sizeof((pref))-1)

static void populateCards(char *filename);
static void setup(void);
static void teardown(void);

static struct Card **cards;
static size_t cardsCapacity = 10;
static size_t numCards = 0;

char *
readCard(FILE *cardFile, const char *sentinel)
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
	filename[strcspn(filename, "\n")] = 0;

	strcpy(path, "example/");
	strcat(path, filename);
	FILE *cardFile = fopen(path, "r");

	if (!cardFile) {
		fprintf(stderr, "Unable to open file \"%s\"\n", path);
		return;
	}

	char line[MAX_CARD_FACE_CHARS];

	while (fgets(line, sizeof(line), cardFile)) {
		if (IS_PREFIX(FRONT_TAG_OPEN, line)) {
			struct Card *currCard = malloc(sizeof(struct Card *));
			currCard->front = readCard(cardFile, FRONT_TAG_CLOSE);

			while (fgets(line, sizeof(line), cardFile)
					&& IS_PREFIX(BACK_TAG_OPEN, line)) {
				// Consume empty space and BACK_TAG_OPEN.
				continue;
			}

			currCard->back = readCard(cardFile, BACK_TAG_CLOSE);

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

//  	for (int i = 0; i < (int) numCards; ++i) {
//  	 	printf("%d: front: %s\n   back: %s\n===========================\n", i, cards[i]->front, cards[i]->back);
//  	}
}

void
teardown(void)
{
	for (int i = 0; i < (int) numCards; ++i) free(cards[i]);
	free(cards);

	return;
}

int
main(void)
{
	setup();
	teardown();
}
