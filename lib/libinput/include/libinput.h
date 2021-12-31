#ifndef __LIBINPUT__
#define __LIBINPUT__

#include <stdio.h>

typedef struct FileStats {
    int lines;
    int longest_line;
} FileStats;

typedef struct Lines {
    char **lines;
    int size;
    int length;
} Lines;

Lines* lines_read(FILE *fp);
void lines_free(Lines *lines);

FileStats* file_stats(FILE *fp);

#endif // __LIBINPUT__
