#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <chrono>

struct TapeConfig {
  std::chrono::milliseconds readDelay;
  std::chrono::milliseconds writeDelay;
  std::chrono::milliseconds moveDelay;
  std::chrono::milliseconds rewindDelay;
};

TapeConfig loadConfig(const std::string& filename);

#endif
