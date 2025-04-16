#include "../include/sortings.h"
#include "../include/comparators.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

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

static void merge(char **arr, size_t left, size_t mid, size_t right,
                  int (*comparator)(const char *, const char *)) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    char **L = (char **)malloc(n1 * sizeof(char *));
    if (!L) {
        fprintf(stderr, "Can't allocate memory\n");
        return;
    }
    char **R = (char **)malloc(n2 * sizeof(char *));
    if (!R) {
        fprintf(stderr, "Can't allocate memory\n");
        return;
    }
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

static void merge_recursion(char **arr, size_t left, size_t right,
                            int (*comparator)(const char *, const char *)) {
    if (left < right) {
        size_t mid = (right + left) / 2;
        merge_recursion(arr, left, mid, comparator);
        merge_recursion(arr, mid + 1, right, comparator);
        merge(arr, left, mid, right, comparator);
    }
}

void sort_merge(char **arr, size_t size, int (*comparator)(const char *, const char *)) {
    if (size > 1) {
        merge_recursion(arr, 0, size - 1, comparator);
    }
}

static size_t partition(char **arr, size_t left, size_t right,
                        int (*comparator)(const char *, const char *)) {
    char *pivot = arr[right];
    size_t i = left;

    for (size_t j = left; j < right; j++) {
        if (comparator(arr[j], pivot) < 0) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }

    swap(&arr[i], &arr[right]);
    return i;
}

static void quick_recurion(char **arr, size_t left, size_t right,
                           int (*comparator)(const char *, const char *)) {
    if (left < right) {
        size_t pivot = partition(arr, left, right, comparator);
        if (pivot > 0) {
            quick_recurion(arr, left, pivot - 1, comparator);
        }
        quick_recurion(arr, pivot + 1, right, comparator);
    }
}

void sort_quick(char **arr, size_t size, int (*comparator)(const char *, const char *)) {
    if (size > 1) {
        quick_recurion(arr, 0, size - 1, comparator);
    }
}

static void reverse_string_array(char *arr[], int size) {
    int i = 0;
    int j = size - 1;

    while (i < j) {
        char *temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;

        i++;
        j--;
    }
}

static size_t get_max_length(char **arr, size_t size) {
    size_t max_len = 0;
    for (size_t i = 0; i < size; i++) {
        size_t len = strlen(arr[i]);
        if (len > max_len) {
            max_len = len;
        }
    }
    return max_len;
}

static char get_char_at_pos(const char *str, size_t pos) {
    if (pos < strlen(str)) {
        return str[pos];
    }
    return 0;
}

void sort_radix(char **arr, size_t size, int (*comparator)(const char *, const char *)) {
    if (size <= 1)
        return;

    size_t max_len = get_max_length(arr, size);

    char **output = (char **)malloc(size * sizeof(char *));
    if (!output) {
        fprintf(stderr, "Can't allocate memory\n");
        return;
    }

    for (int pos = max_len - 1; pos >= 0; --pos) {
        // ASCII
        int count[256] = {0};

        for (size_t i = 0; i < size; i++) {
            char ch = get_char_at_pos(arr[i], pos);
            count[(unsigned char)ch]++;
        }

        for (int i = 1; i < 256; ++i) {
            count[i] += count[i - 1];
        }

        for (int i = size - 1; i >= 0; --i) {
            char ch = get_char_at_pos(arr[i], pos);
            output[count[(unsigned char)ch] - 1] = arr[i];
            count[(unsigned char)ch]--;
        }

        for (size_t i = 0; i < size; ++i) {
            arr[i] = output[i];
        }
    }

    char *a = "a";
    char *b = "b";
    if (comparator(a, b) > 0) {
        reverse_string_array(arr, size);
    }

    free(output);
}
