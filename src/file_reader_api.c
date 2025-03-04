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

char **read_file(const char *filename, size_t line_counted) {
    size_t counter_of_lines = count_lines_in_file(filename);
    if (counter_of_lines == 0) {
        return NULL;
    }

    if (counter_of_lines < line_counted) {
        fprintf(stderr, "There are fewer lines in the file than you entered");
        return NULL;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Can't open file. Check it\n");
        return NULL;
    }

    char **array_of_strings = (char **)malloc(line_counted * sizeof(char *));
    if (!array_of_strings) {
        fprintf(stderr, "Allocation failed\n");
        fclose(file);
        return NULL;
    }

    char buffer[MAX_LINE_LENGTH];
    size_t index = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        array_of_strings[index] = strdup(buffer);
        if (!array_of_strings[index]) {
            fprintf(stderr, "Allocation failed\n");
            fclose(file);
            for (size_t i = 0; i < index; i++)
                free(array_of_strings[i]);
            free(array_of_strings);
            return NULL;
        }
        index++;
        if (index == line_counted) {
            break;
        }
    }
    fclose(file);
    return array_of_strings;
}

void free_file_content(char **array_of_strings, size_t line_counted) {
    for (size_t i = 0; i < line_counted; i++) {
        free(array_of_strings[i]);
    }
    free(array_of_strings);
}
