#pragma once

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <memory>
#include "logging.h"

#undef LOGGING_COMPONENT
#define LOGGING_COMPONENT "StreamBuffer"

namespace toyrpc {

class StreamBuffer {
  const static bool SHRINK_WITH_GET = false;
  const static size_t GROW_SIZE = 1024;
  const static size_t RESERVED_HEADER_SPACE = 64;

public:
  StreamBuffer()
    : buf_(nullptr),
      const_buf_(false),
      pend_(0),
      gpos_(0),
      ppos_(0) {}

  void InitOstream() {
    buf_ = (char*) malloc (RESERVED_HEADER_SPACE * 2);
    const_buf_ = false;
    pend_ = RESERVED_HEADER_SPACE * 2;
    gpos_ = RESERVED_HEADER_SPACE;
    ppos_ = RESERVED_HEADER_SPACE;
  }

  StreamBuffer(const char * buf, size_t size)
    : buf_(const_cast<char*>(buf)),
      const_buf_(true),
      pend_(size),
      gpos_(0),
      ppos_(size) {}

  StreamBuffer(size_t size)
    : buf_((char*)malloc(size)),
      const_buf_(false),
      pend_(size),
      gpos_(0),
      ppos_(0) {}

  ~StreamBuffer() {
    if (!const_buf_) {
      free(buf_);
    }
  }

  void Swap(StreamBuffer &rhs) {
    std::swap(buf_, rhs.buf_);
    std::swap(const_buf_, rhs.const_buf_);
    std::swap(pend_, rhs.pend_);
    std::swap(ppos_, rhs.ppos_);
    std::swap(gpos_, rhs.gpos_);
  }

  char *GetBuf() {
    return buf_ + gpos_;
  }

  void SetBuf(char *buf, size_t size) {
    if (!const_buf_) free(buf_);
    const_buf_ = false;
    buf_ = buf;
    ppos_ = size;
    gpos_ = 0;
    pend_ = size;
  }

  size_t GetSize() {
    return ppos_ - gpos_;
  }

  void Write(const void * buf, size_t size) {
    if (buf_ == nullptr)
      InitOstream();

    size_t new_size = size + ppos_;
    if (new_size > pend_) {
      new_size = std::max(new_size, ppos_ + GROW_SIZE);
      char *new_buf = (char *)std::realloc(buf_, new_size);
      buf_ = new_buf;
      pend_ = new_size;
    }
    memcpy(buf_ + ppos_, buf, size);
    ppos_ += size;
  }

  void Read(void * buf, size_t size) {
    memcpy(buf, buf_ + gpos_, size);
    gpos_ += size;
    if (gpos_ > GROW_SIZE && SHRINK_WITH_GET && !const_buf_) {
      memmove(buf_, buf_ + gpos_, ppos_ - gpos_);
      char * new_buf = (char *)realloc(buf_, pend_ - gpos_);
      buf_ = new_buf;
      pend_ -= gpos_;
      ppos_ -= gpos_;
      gpos_ = 0;
    }
  }

  void WriteHead(const char * buf, size_t size) {
    // assert const_buf_ is false;
    /*
    if (gpos_ < size) {
      size_t new_size = std::max(
          size + ppos_, ppos_ + RESERVED_HEADER_SPACE);
      char * new_buf = (char *)malloc(new_size);

    }
    */
  }

private:
  StreamBuffer(const StreamBuffer& rhs) {};
  StreamBuffer & operator = (const StreamBuffer& rhs) {return *this;}

  char * buf_;
  bool const_buf_;  // const buffers should not be written into
  size_t pend_;  // end of buffer0
  size_t gpos_;  // start of get
  size_t ppos_;  // start of put

  friend class CommAsio;
};

};
