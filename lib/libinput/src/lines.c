#include <stdlib.h>
#include "libinput.h"

static Lines* lines_new(int line_count, int line_length)
{
    Lines *lines = malloc(sizeof(Lines));
    lines->lines = malloc(line_count * sizeof(char*));
    lines->size = line_count;
    lines->length = line_length;
    for (int i = 0; i < line_count; ++i) {
        lines->lines[i] = malloc(sizeof(char) * line_length+1);
    }
    return lines;
}

Lines* lines_read(FILE *fp)
{
    FileStats *stats = file_stats(fp);
    Lines* lines = lines_new(stats->lines, stats->longest_line);
    free(stats);

    int line_num = 0;
    int ch_num = 0;
    char ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            lines->lines[line_num++][ch_num] = '\0';
            ch_num = 0;
        } else {
            lines->lines[line_num][ch_num++] = ch;
        }
    }

    /* Add the last line. If file didn't end on a new-line */
    if (ch_num > 0) {
        lines->lines[line_num][ch_num] = '\0';
    }

    rewind(fp);
    return lines;
}

void lines_print(Lines *lines)
{
	for (size_t i = 0; i < lines->size; ++i) {
		printf("%s\n", lines->lines[i]);
	}
}

void lines_free(Lines *lines)
{
    for (size_t i = 0; i < lines->size; i++) {
        free(lines->lines[i]);
    }
    free(lines->lines);
    free(lines);
}
