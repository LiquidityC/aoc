#include <libinput.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

int main(void)
{
    FILE *fp = fopen("input.txt", "r");
    Lines *lines = lines_read(fp);

    for (int i = 0; i < lines->size; ++i) {
        printf("%s\n", lines->lines[i]);
    }

    lines_free(lines);
    return 0;
}
