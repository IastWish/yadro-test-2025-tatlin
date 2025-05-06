#include "config.hpp"
#include <fstream>
#include <string>
#include <sstream>

TapeConfig loadConfig(const std::string& filename) {
  TapeConfig config{};
  std::ifstream inputf(filename);
  if (!inputf) {
    config.readDelay = std::chrono::milliseconds(100);
    config.writeDelay = std::chrono::milliseconds(100);
    config.moveDelay = std::chrono::milliseconds(50);
    config.rewindDelay = std::chrono::milliseconds(1000);
    return config;
  }

  std::string line;
  while (std::getline(inputf, line)) {
    std::istringstream iss(line);
    std::string key;
    if (std::getline(iss, key, ' ')) {
      int value = 0;
      iss >> value;
      if (key == "readDelay") {
        config.readDelay = std::chrono::milliseconds(value);
      }
      else if (key == "writeDelay") {
        config.writeDelay = std::chrono::milliseconds(value);
      }
      else if (key == "moveDelay") {
        config.moveDelay = std::chrono::milliseconds(value);
      }
      else if (key == "rewindDelay") {
        config.rewindDelay = std::chrono::milliseconds(value);
      }
    }
  }
  return config;
}
