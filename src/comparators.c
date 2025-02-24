#include "../include/comparators.h"
#include <string.h>

int compare_asc(const char *a, const char *b) { return strcmp(a, b); };

int compare_des(const char *a, const char *b) { return strcmp(b, a); };

void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}
