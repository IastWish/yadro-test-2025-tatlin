#ifndef TAPE_HPP
#define TAPE_HPP

#include <vector>
#include <fstream>
#include "itape.hpp"
#include "config.hpp"

class Tape : public ITape {
public:
  Tape(const std::string& filename, const TapeConfig& config, bool createIfNotExists);
  ~Tape();

  int32_t read() override;
  void write(int32_t value) override;
  void moveForward() override;
  void moveBackward() override;
  void rewindForward() override;
  void rewindBackward() override;
  size_t getPosition() const override;
  size_t getSize() const override;

private:
  std::fstream file_;
  std::string filename_;
  size_t position_;
  size_t size_;
  TapeConfig config_;

  void simulateDelay(const std::chrono::milliseconds& delay) const;
  void updateSize();
};

#endif
