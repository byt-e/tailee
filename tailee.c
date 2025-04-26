#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_LINES 10

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    fseek(fp, 0, SEEK_END);
    int cursor = ftell(fp);
    int filesize = cursor;

    int new_lines = 0;
    for (int i = cursor; i > 0 && new_lines != DEFAULT_LINES; --i) {
        fseek(fp, --cursor, SEEK_SET);
        char c = fgetc(fp);
        if (c == '\n') {
            new_lines++;
        }
    }

    int bytes_to_read = filesize - cursor;
    char *lines = malloc(bytes_to_read + 1);

    fseek(fp, cursor, SEEK_SET);
    fread(lines, sizeof(char),  bytes_to_read, fp);
    lines[bytes_to_read] = '\0'; // null-terminate.

    fprintf(stdout, "%s", lines);

    free(lines);
    fclose(fp);

    return EXIT_SUCCESS;
}
