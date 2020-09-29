#include "textbuffer.h"

#include <stdlib.h>

#define CHUNK_SIZE (sizeof(char) * 0x1000)

size_t read_text(textbuffer *buffer, FILE *file) {
  buffer->char_count = 0;
  buffer->internal_buffer = NULL;

  while (!feof(file)) {
    buffer->internal_buffer =
        realloc(buffer->internal_buffer, buffer->char_count + CHUNK_SIZE);
    buffer->char_count += fread(buffer->internal_buffer + buffer->char_count,
                                sizeof(char), CHUNK_SIZE, file);
  }

  buffer->internal_buffer =
      realloc(buffer->internal_buffer, buffer->char_count + 1);
  buffer->internal_buffer[buffer->char_count] = '\0';

  return buffer->char_count;
}

void dump_textbuffer(textbuffer *buffer, FILE *stream) {
  fputs(buffer->internal_buffer, stream);
  fputc('\n', stream);
}

void free_textbuffer(textbuffer *buffer) {
  free(buffer->internal_buffer);
  buffer->char_count = 0;
}
