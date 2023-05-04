#include <stdlib.h>
#include <stdio.h>
#include "libinput.h"

static FileStats* stats_new()
{
    return (FileStats*) malloc(sizeof(FileStats));
}

FileStats* file_stats(FILE *fp)
{
    FileStats *stats = stats_new();
    stats->lines = 0;
    stats->longest_line = 0;

    size_t line_len = 0;
    int ch = 0;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            stats->lines++;
            if (line_len > stats->longest_line) {
                stats->longest_line = line_len;
            }
            line_len = 0;
        } else {
            line_len += 1;
        }
    }
    rewind(fp);
    return stats;
}
