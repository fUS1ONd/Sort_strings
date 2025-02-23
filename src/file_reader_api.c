#include "../include/file_reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

char **read_file(const char *filename, size_t line_count) {
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