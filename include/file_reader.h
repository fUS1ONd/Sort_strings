#include <stddef.h>

#pragma once

char **read_file(const char *filename, size_t line_counted);

void free_file_content(char **array_of_strings, size_t line_counted);
