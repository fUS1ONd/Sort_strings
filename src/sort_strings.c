#include "../include/comparators.h"
#include "../include/file_reader.h"
#include "../include/sortings.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERREXCODE -1

#define MAX_LINE_LENGTH 1000

/* usage : sort_strings num_of_str's input.txt sort comparer */
int check_cmd_args(int argc, char const *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Incorrect number of parameters:\nusage : sort_strings num_of_str's "
                        "input.txt sort comparer\n");
        return ERREXCODE;
    }

    char *end;
    if ((strtol(argv[1], &end, 10) <= 0) || !(*argv[1] != '\0' && *end == '\0')) {
        fprintf(stderr, "Incorrect parameter of number of lines\n");
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

int main(int argc, char const *argv[]) {

    int result_of_checking = check_cmd_args(argc, argv);
    if (result_of_checking == ERREXCODE) {
        return ERREXCODE;
    }

    const char *filename = argv[2];
    size_t line_count = atoi(argv[1]);
    char **arr = read_file(filename, line_count);
    if (!arr) {
        fprintf(stderr, "Can't open file\n");
        return ERREXCODE;
    }

    if (strcmp(argv[3], "bubble") == 0) {
        if (strcmp(argv[4], "asc") == 0)
            sort_bubble(arr, line_count, compare_asc);
        else
            sort_bubble(arr, line_count, compare_des);
    } else if (strcmp(argv[3], "insertion") == 0) {
        if (strcmp(argv[4], "asc") == 0)
            sort_insertion(arr, line_count, compare_asc);
        else
            sort_insertion(arr, line_count, compare_des);
    } else if (strcmp(argv[3], "merge") == 0) {
        if (strcmp(argv[4], "asc") == 0)
            sort_merge(arr, line_count, compare_asc);
        else
            sort_merge(arr, line_count, compare_des);
    } else if (strcmp(argv[3], "quick") == 0) {
        if (strcmp(argv[4], "asc") == 0)
            sort_quick(arr, line_count, compare_asc);
        else
            sort_quick(arr, line_count, compare_des);
    } else if (strcmp(argv[3], "radix") == 0) {
        if (strcmp(argv[4], "asc") == 0)
            sort_radix(arr, line_count, compare_asc);
        else
            sort_radix(arr, line_count, compare_des);
    }

    for (size_t i = 0; i < line_count; i++) {
        printf("%s\n", arr[i]);
    }

    free_file_content(arr, line_count);
    return 0;
}
