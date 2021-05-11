#ifndef MAIN_H
#define MAIN_H

#include "defs.h"
#include <stdbool.h>
#include <stdlib.h>

extern bool shouldRun; 
extern size_t numCards;
extern struct Card **cards;

int populateCards(char *fileName);

#endif /* MAIN_H */
