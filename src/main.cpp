#include <iostream>
#include <fstream>
#include <string>
#include "config.hpp"
#include "tape_sorter.hpp"
#include "tape.hpp"
#include "converter.hpp"

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Required command: " << argv[0] << " <input.txt|.bin> <output.txt|.bin> <config.txt> <mem_MB>\n";
    return 1;
  }
  try {
    std::string inputF = argv[1];
    std::string outputF = argv[2];
    std::string configF = argv[3];
    size_t memLim = static_cast<size_t>(std::stoull(argv[4]));

    TapeConfig config = loadConfig(configF);
    TapeSorter sorter(memLim);

    std::string binIn = inputF;
    bool isTxtIn = inputF.size() > 4 && inputF.substr(inputF.size() - 4) == ".txt";
    bool isTxtOut = outputF.size() > 4 && outputF.substr(outputF.size() - 4) == ".txt";

    std::string actualInputBin;
    std::string actualOutputBin;
    if (isTxtIn) {
      actualInputBin = inputF.substr(0, inputF.size() - 4) + ".bin";
    }
    else {
      actualInputBin = inputF;
    }
    if (isTxtOut) {
      actualOutputBin = outputF.substr(0, outputF.size() - 4) + ".bin";
    }
    else {
      actualOutputBin = outputF;
    }

    if (isTxtIn) {
      txtToBin(inputF, actualInputBin);
    }
    Tape inputTape(actualInputBin, config, false);
    Tape outputTape(actualOutputBin, config, true);

    sorter.sort(inputTape, outputTape);
    if (isTxtOut) {
      binToTxt(actualOutputBin, outputF);
    }
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
