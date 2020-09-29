#ifndef AAVM_TEXTBUFFER_H
#define AAVM_TEXTBUFFER_H

#include <stddef.h>
#include <stdio.h>

typedef struct {
  char *internal_buffer;
  size_t char_count;
} textbuffer;

size_t read_text(textbuffer *buffer, FILE *file);
void dump_textbuffer(textbuffer *buffer, FILE *stream);
void free_textbuffer(textbuffer *buffer);

#endif
