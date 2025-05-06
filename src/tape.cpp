#include "tape.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <filesystem>

Tape::Tape(const std::string& filename, const TapeConfig& config, bool createIfNotExists):
  filename_(filename),
  config_(config),
  position_(0),
  size_(0)
{
  if (createIfNotExists) {
    std::ofstream out(filename_, std::ios::binary);
  }
  file_.open(filename_, std::ios::in | std::ios::out | std::ios::binary);
  if (!file_) {
    throw std::runtime_error("Failed to open Tape file");
  }
  updateSize();
}

Tape::~Tape() {
  if (file_.is_open()) {
    file_.close();
  }
}

int32_t Tape::read() {
  simulateDelay(config_.readDelay);
  int32_t value = 0;
  file_.seekg(position_ * sizeof(int32_t));
  file_.read(reinterpret_cast<char*>(&value), sizeof(value));
  return value;
}

void Tape::write(int32_t value) {
  simulateDelay(config_.writeDelay);
  file_.seekp(position_ * sizeof(int32_t));
  file_.write(reinterpret_cast<const char*>(&value), sizeof(value));
  updateSize();
}

void Tape::moveForward() {
  simulateDelay(config_.moveDelay);
  ++position_;
}

void Tape::moveBackward() {
  simulateDelay(config_.moveDelay);
  if (position_ > 0) {
    --position_;
  }
}

void Tape::rewindForward() {
  simulateDelay(config_.rewindDelay);
  position_ = size_ ? size_ - 1 : 0;
}

void Tape::rewindBackward() {
  simulateDelay(config_.rewindDelay);
  position_ = 0;
}

size_t Tape::getPosition() const {
  return position_;
}

size_t Tape::getSize() const {
  return size_;
}

void Tape::simulateDelay(const std::chrono::milliseconds& delay) const {
  std::this_thread::sleep_for(delay);
}

void Tape::updateSize() {
  file_.seekg(0, std::ios::end);
  size_ = file_.tellg() / sizeof(int32_t);
  file_.seekg(0, std::ios::beg);
}
