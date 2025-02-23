#include "../include/sortings.h"
#include "../include/comparators.h"
#include <stdio.h>
#include <string.h>

void sort_bubble(char **arr, size_t size, int (*comparator)(const char *, const char *)) {
    for (size_t i = 0; i < size - 1; ++i) {
        for (size_t j = 0; j < size - i - 1; ++j) {
            if (comparator(arr[j], arr[j + 1]) > 0) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void sort_insertion(char **arr, size_t size, int (*comparator)(const char *, const char *)) {
    for (size_t i = 1; i < size; ++i) {
        for (size_t j = i; j > 0 && comparator(arr[j - 1], arr[j]) > 0; --j) {
            swap(&arr[j - 1], &arr[j]);
        }
    }
}