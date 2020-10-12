#pragma once

#include "streambuffer.h"
#include <atomic>
#include <bits/stdint-uintn.h>
#include <cstdlib>
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
  ZmqEP() {}
  ZmqEP(const std::string& ip, uint16_t port)
    : ip_string_(ip),
      port_(port) {
    uint32_t ip_binary = 0;
    size_t pos = 0;
    for (int i = 0; i < 4; i++) {
      uint8_t d = atoi(ip_string_.c_str() + pos);
      ip_binary = ((ip_binary << 8) | d);
      if (i < 3) {
        pos = ip_string_.find('.', pos);
        if (pos == ip_string_.npos) {
          REMMY_ABORT("Error parsing ip address %s", ip_string_.c_str());
        }
        pos++;
      }
    }
    hash_ = (((uint64_t)ip_binary << 16) | port_);
  }

  ZmqEP(uint64_t hash) {
    hash_ = hash;
    port_ = hash_ & 0xffff;
  }

  void SetPort(uint16_t port) {
    port_ = port;
    hash_ = ((hash_ >> 16) << 16) | port;
  }

  bool operator==(const ZmqEP& rhs) const {
    return hash_ == rhs.hash_;
  }

  std::string ToString() const {
    if (ip_string_.empty()) {
      uint32_t binary = (uint32_t)(hash_ >> 16);
      for (int i = 0; i < 4; i++) {
        uint8_t d = (binary & 0xff000000) >> 24;
        ip_string_ += std::to_string(d);
        if (i != 3) ip_string_ += ".";
        binary = (binary << 8);
      }
    }
    return std::string("tcp://") + ip_string_ + ":" + std::to_string(port_);
  }

  uint64_t Hash() const {
    return hash_;
  }

  uint16_t Port() const {
    return port_;
  }

  void Serialize(StreamBuffer& buf) const {
    buf.Write(&hash_, sizeof(hash_));
  }

  void Deserialize(StreamBuffer& buf) {
    buf.Read(&hash_, sizeof(hash_));
    port_ = hash_ & 0xffff;
  }
};

template <>
inline const std::string EPToString<ZmqEP>(const ZmqEP &ep) {
  return ep.ToString();
}

};

namespace toyrpc {

class CommZmq : public CommBase<ZmqEP> {
  
}

};
