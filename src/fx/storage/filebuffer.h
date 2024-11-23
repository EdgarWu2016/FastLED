
#pragma once

#include <stdint.h>

#include "file_system.h"
#include "fl/ptr.h"
#include "crgb.h"
#include "namespace.h"

FASTLED_NAMESPACE_BEGIN

FASTLED_SMART_PTR(FileBuffer);
FASTLED_SMART_PTR(FileHandle);

class FileBuffer: public fl::Referent {
 public:
  FileBuffer(FileHandleRef file);
  virtual ~FileBuffer();
  void rewindToStart();
  bool available() const;
  size_t bytesLeft() const;
  int32_t FileSize() const;
  bool seek(uint32_t pos);
  uint32_t Position() const;

  // Reads the next byte, else -1 is returned for end of buffer.
  int16_t read();
  size_t read(uint8_t* dst, size_t n);
  size_t readCRGB(CRGB* dst, size_t n);

 private:
  void ResetBuffer();
  void RefillBufferIfNecessary();
  void RefillBuffer();

  // Experimentally found to be as fast as larger values.
  static const int kBufferSize = 64;
  uint8_t mBuffer[kBufferSize] = {0};
  int16_t mCurrIdx;
  int16_t mLength;

  FileHandleRef mFile;
};

FASTLED_NAMESPACE_END
