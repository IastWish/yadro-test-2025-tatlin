#include "converter.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>

void txtToBin(const std::string& txtFile, const std::string& binFile) {
  std::ifstream in(txtFile);
  std::ofstream out(binFile, std::ios::binary);
  int32_t value = 0;
  while (in >> value) {
    out.write(reinterpret_cast<char*>(&value), sizeof(value));
  }
}

void binToTxt(const std::string& binFile, const std::string& txtFile) {
  std::ifstream in(binFile, std::ios::binary);
  std::ofstream out(txtFile);

  if (!in || !out) {
    throw std::runtime_error("Failed to open files in binToTxt");
  }

  int32_t value;
  bool first = true;

  while (in.read(reinterpret_cast<char*>(&value), sizeof(value))) {
    if (!first) {
      out << " ";
    }
    out << value;
    first = false;
  }
}
