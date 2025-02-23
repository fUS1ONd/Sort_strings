#include "../include/file_reader.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

char **read_file(const char *filename, size_t line_count) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Can't open file\n");
        return NULL;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        fprintf(stderr, "Can't get filesize\n");
        close(fd);
        return NULL;
    }
    size_t file_size = st.st_size;

    char *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        fprintf(stderr, "Can't map the file into memory\n");
        close(fd);
        return NULL;
    }

    size_t count = 0;
    for (size_t i = 0; i < file_size; i++) {
        if (file_data[i] == '\n')
            count++;
        if (count == line_count)
            break;
    }
    if (file_size > 0 && file_data[file_size - 1] != '\n' && count < line_count)
        count++;

    if (count < line_count) {
        fprintf(stderr, "There are fewer lines in the file than you entered\n");
        close(fd);
        return NULL;
    }

    char **arr = (char **)malloc(line_count * sizeof(char *));
    if (!arr) {
        fprintf(stderr, "Can't allocate memory\n");
        munmap(file_data, file_size);
        close(fd);
        return NULL;
    }

    size_t index = 0;
    char *line_start = file_data;
    for (size_t i = 0; i < file_size && index < line_count; i++) {
        if (file_data[i] == '\n' || i == file_size - 1) {
            size_t line_length = (file_data + i) - line_start;
            if (i == file_size - 1 && file_data[i] != '\n') {
                line_length++;
            }

            arr[index] = (char *)malloc((line_length + 1) * sizeof(char));
            if (!arr[index]) {
                fprintf(stderr, "Can't allocate memory for lines\n");
                for (size_t j = 0; j < index; j++)
                    free(arr[j]);
                free(arr);
                munmap(file_data, file_size);
                close(fd);
                return NULL;
            }
            strncpy(arr[index], line_start, line_length);
            arr[index][line_length] = '\0';

            index++;
            line_start = file_data + i + 1;
        }
    }

    munmap(file_data, file_size);
    close(fd);
    return arr;
}

void free_file_content(char **arr, size_t line_count) {
    for (size_t i = 0; i < line_count; i++) {
        free(arr[i]);
    }
    free(arr);
}
