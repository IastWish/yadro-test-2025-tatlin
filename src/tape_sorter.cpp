#include "tape_sorter.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <exception>

TapeSorter::TapeSorter(size_t memoryLimitBytes):
  memoryLimitBytes_(memoryLimitBytes)
{}

void TapeSorter::sort(Tape& inputTape, Tape& outputTape) {
  size_t tmpFilesCount = splitToSortedTmpTapes(inputTape, memoryLimitBytes_);
  mergeTmpTapes(tmpFilesCount, outputTape, memoryLimitBytes_);
  outputTape.rewindBackward();
}

size_t TapeSorter::splitToSortedTmpTapes(Tape& inputTape, size_t memoryLimitBytes) {
  inputTape.rewindBackward();
  size_t maxTmpFileSize = memoryLimitBytes / sizeof(int32_t);
  size_t total = inputTape.getSize();
  size_t processed = 0;
  size_t tmpTapeIndex = 0;

  while (processed < total) {
    std::vector<int32_t> buffer;
    size_t count = std::min(maxTmpFileSize, total - processed);
    for (size_t i = 0; i < count; ++i) {
      buffer.push_back(inputTape.read());
      if (i + 1 < count) {
        inputTape.moveForward();
      }
    }
    std::sort(buffer.begin(), buffer.end());

    std::string tmpFile = "/tmp/tape" + std::to_string(tmpTapeIndex) + ".bin";
    tmpTapeIndex++;
    std::ofstream out(tmpFile, std::ios::binary);
    for (int32_t el : buffer) {
      out.write(reinterpret_cast<const char*>(&el), sizeof(el));
    }
    processed += count;
    if (processed < total) {
      inputTape.moveForward();
    }
  }
  return tmpTapeIndex;
}

void TapeSorter::mergeTmpTapes(size_t tmpFilesCount, Tape& outputTape, size_t memoryLimitBytes) {
  while (true) {
    int32_t minValue = std::numeric_limits< int32_t >::max();
    size_t minIndex = 0;
    bool found = false;

    for (size_t i = 0; i < tmpFilesCount; ++i) {
      std::string fname = "/tmp/tape" + std::to_string(i) + ".bin";

      std::ifstream in(fname, std::ios::binary);
      if (!in.is_open()) {
        continue;
      }

      int32_t value = std::numeric_limits< int32_t >::max();
      if (in.read(reinterpret_cast<char*>(&value), sizeof(int32_t))) {
        if (!found || value < minValue) {
          minValue = value;
          minIndex = i;
          found = true;
        }
      }
      else {
        in.close();
        std::remove(fname.c_str());
      }
    }

    if (!found) {
      break;
    }

    outputTape.write(minValue);
    outputTape.moveForward();

    std::string fileToDelete = "/tmp/tape" + std::to_string(minIndex) + ".bin";
    removeFirstEl(fileToDelete, memoryLimitBytes);
  }
  outputTape.rewindBackward();
}

void TapeSorter::removeFirstEl(const std::string& filename, size_t memoryLimitBytes) {
  try {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
      std::cerr << "Failed to open file: " << filename << "\n";
      return;
    }

    std::string tmpFilename = filename + ".tmp";
    std::ofstream out(tmpFilename, std::ios::binary);
    if (!out) {
      std::cerr << "Failed to create temp file: " << tmpFilename << "\n";
      return;
    }

    in.seekg(sizeof(int32_t), std::ios::beg);

    size_t bufferSize = memoryLimitBytes - (memoryLimitBytes % sizeof(int32_t));
    char* buffer = new char[bufferSize];

    while (in) {
      in.read(buffer, bufferSize);
      std::streamsize bytesRead = in.gcount();
      if (bytesRead > 0) {
        out.write(buffer, bytesRead);
      }
    }

    delete[] buffer;
    in.close();
    out.close();

    std::remove(filename.c_str());
    int res = std::rename(tmpFilename.c_str(), filename.c_str());
    if (res != 0) {
      std::perror(("Rename failed: " + tmpFilename + " to " + filename).c_str());
    }
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return;
  }
}
