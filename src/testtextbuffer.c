#include "textbuffer.h"

#include <stdio.h>

void usage() { printf("usage: testtextbuffer [filename]\n"); }

int main(int argc, char **argv) {
  FILE *file;
  char *filename;
  textbuffer buffer;

  if (argc < 2) {
    usage();
    return -1;
  }

  filename = argv[1];
  file = fopen(filename, "r");
  if (!file) {
    printf("unable to open file \"%s\"\n", filename);
    return -1;
  }

  read_text(&buffer, file);
  dump_textbuffer(&buffer, stdout);
  free_textbuffer(&buffer);

  fclose(file);
}
