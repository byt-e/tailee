#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_LINES 10

int parse_line_count(const char *arg, int *out);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-n 10] <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int line_count = DEFAULT_LINES;
    const char *filename;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: %s [-n <line_count>] <filename>\n", argv[0]);
            printf("\t-n N\t\tOutput the last N lines (default: 10)\n");
            printf("\t--help\t\tShow this help message\n");
            return EXIT_SUCCESS;
        }

        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            if (parse_line_count(argv[++i], &line_count) != 0) {
                fprintf(stderr, "Invalid value for -n: %s\n", argv[i]);
                return EXIT_FAILURE;
            }
        } else {
            filename = argv[i];
        }
    }

    if (!filename) {
        fprintf(stderr, "Missing filename. Use --help for usage.\n");
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    fseek(fp, 0, SEEK_END);
    int cursor = ftell(fp);
    int filesize = cursor;

    int new_lines = 0;
    for (int i = cursor; i > 0 && new_lines != line_count; --i) {
        fseek(fp, --cursor, SEEK_SET);
        char c = fgetc(fp);
        if (c == '\n') {
            new_lines++;
        }
    }

    int bytes_to_read = filesize - cursor;
    char *lines = malloc(bytes_to_read + 1);
    if (!lines) {
        perror("malloc");
        fclose(fp);
        return EXIT_FAILURE;
    }

    fseek(fp, cursor, SEEK_SET);
    fread(lines, sizeof(char),  bytes_to_read, fp);
    lines[bytes_to_read] = '\0'; // null-terminate.

    fprintf(stdout, "%s", lines);

    free(lines);
    fclose(fp);

    return EXIT_SUCCESS;
}

int parse_line_count(const char *arg, int *out) {
    char *endptr;
    long val = strtol(arg, &endptr, 10);

    if (*endptr != '\0' || val < 1) {
        return -1;
    }

    *out = (int) val;

    return 0;
}
