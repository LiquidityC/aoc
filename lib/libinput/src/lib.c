#include <stdlib.h>
#include "libinput.h"

static FileStats* stats_new()
{
    return (FileStats*) malloc(sizeof(FileStats));
}

void lines_free(Lines *lines)
{
    for (int i = 0; i < lines->size; i++) {
        free(lines->lines[i]);
    }
    free(lines->lines);
    free(lines);
}

FileStats* file_stats(FILE *fp)
{
    FileStats *stats = stats_new();
    stats->lines = 0;
    stats->longest_line = 0;

    int line_len = 0;
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

Lines* read_lines(FILE *fp)
{
    FileStats *stats = file_stats(fp);
    Lines *lines = malloc(sizeof(Lines));
    lines->lines = malloc(stats->lines * sizeof(char*));
    for (int i = 0; i < stats->lines; ++i) {
        lines->lines[i] = malloc(sizeof(char) * stats->longest_line+1);
    }
    lines->size = stats->lines;
    free(stats);

    int line_num = 0;
    int ch_num = 0;
    char ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            lines->lines[line_num][ch_num] = '\0';
            line_num += 1;
            ch_num = 0;
        } else {
            lines->lines[line_num][ch_num] = ch;
            ch_num += 1;
        }
    }
    rewind(fp);
    return lines;
}
