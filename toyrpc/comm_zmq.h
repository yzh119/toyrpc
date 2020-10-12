#pragma once

#include <atomic>
#include <exception>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "zmq.hpp"

#include "concurrent_queue.h"
#include "logging.h"
#include "message.h"
#include "comm.h"

namespace toyrpc {

class ZmqEP {
  mutable std::string ip_string_;
  uint16_t port_;
  uint64_t hash_;
public:
private:
};

};

namespace toyrpc {

class CommZmq : public CommBase<ZmqEP> {
  
}

};
