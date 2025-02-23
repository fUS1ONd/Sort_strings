#include "../include/comparators.h"
#include "../include/sortings.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERREXCODE -1

#define MAX_LINE_LENGTH 1000
#define PRINT_LIMIT 100

/* usage : sort_strings num_of_str's input.txt sort comparer */
int check_cmd_args(int argc, char const *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Incorrect number of parameters\n");
        return ERREXCODE;
    }

    if (argv[1] < 0) {
        fprintf(stderr, "Incorrect parameter of number of lines\n");
        return ERREXCODE;
    }

    const char *filename = argv[2];
    int len = strlen(filename);
    if (len <= 4 || strcmp(filename + len - 4, ".txt") != 0) {
        fprintf(stderr, "Incorrect parameter of filename\n");
        return ERREXCODE;
    }

    const char *AvaiableOptions[] = {"bubble", "insertion", "merge", "quick", "radix"};
    int size = sizeof(AvaiableOptions) / sizeof(AvaiableOptions[0]);
    int cntFails = 0;
    for (int i = 0; i < size; ++i) {
        if (strcmp(argv[3], AvaiableOptions[i]) == 0)
            break;
        cntFails += (strcmp(argv[3], AvaiableOptions[i]) != 0) ? 1 : 0;
    }
    if (cntFails == 5) {
        fprintf(stderr, "Incorrect parameter of type of sorting\n");
        return ERREXCODE;
    }

    if (strcmp(argv[4], "des") != 0 && strcmp(argv[4], "asc") != 0) {
        fprintf(stderr, "Incorrect comparer parameter\n");
        return ERREXCODE;
    }
    return 0;
}

void free_string_array(char **arr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        free(arr[i]);
    }
    free(arr);
}

int main(int argc, char const *argv[]) {
    printf("wc -l example.txt\n");

    int resultOfChecking = check_cmd_args(argc, argv);
    if (resultOfChecking == ERREXCODE) {
        return ERREXCODE;
    }

    FILE *file = fopen(argv[2], "r");
    if (!file) {
        fprintf(stderr, "Can't open file. Check it\n");
        return ERREXCODE;
    }

    int line_count = atoi(argv[1]);
    char **arr = (char **)malloc(line_count * sizeof(char *));
    if (!arr) {
        fprintf(stderr, "Allocation failed\n");
        fclose(file);
        return ERREXCODE;
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
            return ERREXCODE;
        }
        index++;
    }
    fclose(file);

    sort_bubble(arr, line_count, compare_asc);
    for (size_t i = 0; i < line_count; i++) {
        printf("%s\n", arr[i]);
    }

    free_string_array(arr, line_count);
    return 0;
}
