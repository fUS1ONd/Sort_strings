#include <stddef.h>

#pragma once

char **read_file(const char *filename, size_t line_count);

void free_file_content(char **lines, size_t line_count);
