#include "libinput.h"

int count_lines(FILE *fp)
{
    int count = 0;
    int ch = 0;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            count += 1;
        }
    }
    rewind(fp);
    return count;
}
