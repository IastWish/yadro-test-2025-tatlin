#ifndef ITAPE_HPP
#define ITAPE_HPP

#include <cstdint>
#include <cstddef>

class ITape {
public:
  virtual ~ITape() = default;
  virtual int32_t read() = 0;
  virtual void write(int32_t value) = 0;
  virtual void moveForward() = 0;
  virtual void moveBackward() = 0;
  virtual void rewindForward() = 0;
  virtual void rewindBackward() = 0;
  virtual size_t getPosition() const = 0;
  virtual size_t getSize() const = 0;
};

#endif
