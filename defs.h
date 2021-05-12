#ifndef DEFS_H
#define DEFS_H

#include "cmd.h"
#include <string.h>
#include <stdlib.h>

/* Macros */
#define IS_PREFIX(pref, str) !strncmp((str), (pref), sizeof((pref))-1)
#define STRIP(s) (s)[strlen(s) - 1] = 0

struct Card {
	const char *front;
	const char *back;
};

static const char * const STARTUP_MESSAGE = "Welcome to Simple-fc.";

/* Text parameters */
static const int MAX_CMD_ARGS 		 = 4;
static const size_t MAX_CMD_LENGTH	 = 128;
static const int MAX_FILEPATH_LENGTH = 256;
static const int MAX_CARD_FACE_CHARS = 1024;

/* Tags */
static const char * const FRONT_TAG_OPEN  = "<front>";
static const char * const FRONT_TAG_CLOSE = "</front>";
static const char * const BACK_TAG_OPEN   = "<back>";
static const char * const BACK_TAG_CLOSE  = "</back>";

/* Command mapping */
static struct {
	const char *name;
	int (*func) (int nargs, char **args);
} const command_table[] = {
	{ "h", 			cmd_help  		},
	{ "help", 		cmd_help  		},
	{ "?", 			cmd_help  		},
	{ "L", 			cmd_load  		},
	{ "l", 			cmd_learn 		},
	{ "p", 			cmd_practice 	},
	{ "t", 			cmd_test 		},
	{ "q", 			cmd_quit  		},
	{ "quit",		cmd_quit  		},
	{ "exit", 		cmd_quit  		},
	{ "halt", 		cmd_quit  		},
	{ NULL, 		NULL			}
};

#endif /* DEFS_H */

