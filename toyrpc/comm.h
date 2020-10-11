#pragma once

#include <functional>
#include <memory>
#include <string>
#include "message.h"

namespace toyrpc {

enum class CommErrors {
  SUCCESS = 0,
  UNKNOWN = 1,
  CONNECTION_REFUSED = 2,
  CONNECTION_ABORTED = 3,
  SEND_ERROR = 4,
  RECEIVE_ERROR = 5
};

const static uint32_t PKG_MAGIC_HEAD = 0x19961111;

class CommBase {
public:
  typedef Message<
}

};
