#include "textbuffer.h"

#include <fstream>
#include <iostream>

using namespace aavm;

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "usage: testtextbuffer [filename]\n";
    return -1;
  }

  std::fstream filestream{argv[1], std::ios_base::in};
  Charbuffer buffer{filestream};
  buffer.dump(std::cout);
  std::cout << "\n";

  return 0;
}
