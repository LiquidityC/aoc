#ifndef __LIBINPUT__
#define __LIBINPUT__

#include <stdio.h>
#include <stdint.h>

typedef struct FileStats {
    size_t lines;
    size_t longest_line;
} FileStats;

/** Struct containing the lines of a file */
typedef struct Lines {
    char **lines;	/**< The lines */
    size_t size;		/**< The number of lines */
    size_t length;		/**< The lenght of the longest line */
} Lines;

/** Read lines from file path */
Lines* lines_read_file(const char *fpath);

/** Read lines from a file handle */
Lines* lines_read(FILE *fp);

/** Print all the lines */
void lines_print(Lines *lines);

/** Destroy the lines */
void lines_free(Lines *lines);

FileStats* file_stats(FILE *fp);

#endif // __LIBINPUT__
