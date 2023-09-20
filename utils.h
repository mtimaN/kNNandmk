#pragma once
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define ERR "Malloc failed!\n"

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)
