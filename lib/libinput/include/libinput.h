#include <stdio.h>

typedef struct FileStats {
    int lines;
    int longest_line;
} FileStats;

typedef struct Lines {
    char **lines;
    int size;
} Lines;

void lines_free(Lines *lines);

FileStats* file_stats(FILE *fp);
Lines* read_lines(FILE *fp);
