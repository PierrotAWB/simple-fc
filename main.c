#include "defs.h"
#include <stdio.h>

static void setup(void);

void
setup(void)
{
	/* Read from file; path temporarily hard-coded */
	FILE *catalogue;
	char buff[255];

	catalogue = fopen("example/catalogue.txt", "r");
	fscanf(catalogue, "%s", buff);
	printf("1: %s\n", buff);
	fclose(catalogue);
}

int
main(void)
{
	setup();
}
