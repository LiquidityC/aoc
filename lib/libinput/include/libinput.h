#ifndef __LIBINPUT__
#define __LIBINPUT__

#include <stdio.h>

typedef struct FileStats {
    int lines;
    int longest_line;
} FileStats;

/** Struct containing the lines of a file */
typedef struct Lines {
    char **lines;	/**< The lines */
    int size;		/**< The number of lines */
    int length;		/**< The lenght of the longest line */
} Lines;

/** Read lines from a file handle */
Lines* lines_read(FILE *fp);

/** Print all the lines */
void lines_print(Lines *lines);

/** Destroy the lines */
void lines_free(Lines *lines);

FileStats* file_stats(FILE *fp);

#endif // __LIBINPUT__
