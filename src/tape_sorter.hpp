#ifndef TAPE_SORTER_HPP
#define TAPE_SORTER_HPP

#include "tape.hpp"
#include <vector>
#include <string>
#include <cstddef>

class TapeSorter {
public:
  TapeSorter(size_t memoryLimitBytes);
  void sort(Tape& input, Tape& output);

private:
  size_t memoryLimitBytes_;
  size_t splitToSortedTmpTapes(Tape& input, size_t memoryLimitBytes);
  void mergeTmpTapes(size_t tmpFilesCount, Tape& outputTape, size_t memoryLimitBytes);
  void removeFirstEl(const std::string& filename, size_t memoryLimitBytes);
};

#endif
