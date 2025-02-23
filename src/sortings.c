#include "../include/sortings.h"
#include "../include/comparators.h"
#include <stdio.h>
#include <stdlib.h>
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

void merge(char **arr, size_t left, size_t mid, size_t right,
           int (*comparator)(const char *, const char *)) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    char **L = (char **)malloc(n1 * sizeof(char *));
    char **R = (char **)malloc(n2 * sizeof(char *));

    for (size_t i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (size_t j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    size_t i = 0;
    size_t j = 0;
    size_t k = left;
    while (i < n1 && j < n2) {
        if (comparator(L[i], R[j]) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void merge_recursion(char **arr, size_t left, size_t right,
                     int (*comparator)(const char *, const char *)) {
    if (left < right) {
        size_t mid = (right + left) / 2;
        merge_recursion(arr, left, mid, comparator);      // Сортируем левую половину
        merge_recursion(arr, mid + 1, right, comparator); // Сортируем правую половину
        merge(arr, left, mid, right, comparator);         // Сливаем отсортированные половины
    }
}

void sort_merge(char **arr, size_t size, int (*comparator)(const char *, const char *)) {
    if (size > 1) {
        merge_recursion(arr, 0, size - 1, comparator);
    }
}
