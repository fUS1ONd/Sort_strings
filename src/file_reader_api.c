#include "../include/file_reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

size_t count_lines_in_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Can't open file. Check it\n");
        return 0;
    }

    size_t line_count = 0;
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), file)) {
        line_count++;
    }

    fclose(file);
    return line_count;
}

char **read_file(const char *filename, size_t line_count) {
    size_t actual_line_count = count_lines_in_file(filename);
    if (actual_line_count == 0) {
        return NULL;
    }

    if (actual_line_count < line_count) {
        fprintf(stderr, "There are fewer lines in the file than you entered");
        return NULL;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Can't open file. Check it\n");
        return NULL;
    }

    char **arr = (char **)malloc(line_count * sizeof(char *));
    if (!arr) {
        fprintf(stderr, "Allocation failed\n");
        fclose(file);
        return NULL;
    }

    char buffer[MAX_LINE_LENGTH];
    size_t index = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        arr[index] = strdup(buffer);
        if (!arr[index]) {
            fprintf(stderr, "Allocation failed\n");
            fclose(file);
            for (size_t i = 0; i < index; i++)
                free(arr[i]);
            free(arr);
            return NULL;
        }
        index++;
        if (index == line_count) {
            break;
        }
    }
    fclose(file);
    return arr;
}

void free_file_content(char **arr, size_t line_count) {
    for (size_t i = 0; i < line_count; i++) {
        free(arr[i]);
    }
    free(arr);
}
