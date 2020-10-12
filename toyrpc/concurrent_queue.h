#pragma once

#include <condition_variable>
#include <limits>
#include <list>
#include <mutex>

#include "logging.h"

namespace toyrpc {

template <class T>
class ConcurrentQueue {
public:

private:
  size_t max_elements_;
  std::list<T> queue_;
  std::mutex mutex_;
  std::condition_variable cv_;
  bool exit_now_;
};

};
