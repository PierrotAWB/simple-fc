#ifndef DEFS_H
#define DEFS_H

struct Card {
	const char *front;
	const char *back;
};

static const int MAX_FILEPATH_LENGTH = 256;
static const int MAX_CARD_FACE_CHARS = 1024;

static const char *FRONT_TAG_OPEN  = "<front>";
static const char *FRONT_TAG_CLOSE = "</front>";
static const char *BACK_TAG_OPEN   = "<back>";
static const char *BACK_TAG_CLOSE  = "</back>";

#endif /* DEFS_H */

