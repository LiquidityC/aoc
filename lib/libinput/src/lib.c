#include <stdlib.h>
#include <stdio.h>
#include "libinput.h"

static FileStats* stats_new()
{
    return (FileStats*) malloc(sizeof(FileStats));
}

static void count_line(FileStats *stats, size_t line_len)
{
    stats->lines++;
    if (line_len > stats->longest_line) {
        stats->longest_line = line_len;
    }
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
            count_line(stats, line_len);
            line_len = 0;
        } else {
            line_len += 1;
        }
    }

    /* If the file didn't end with a new-line then count the last line */
    if (line_len > 0) {
        count_line(stats, line_len);
    }
    rewind(fp);
    return stats;
}
