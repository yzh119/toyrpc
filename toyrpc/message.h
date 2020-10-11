#pragma once

#include <list>
#include "streambuffer.h"
#include "datatypes.h"

namespace toyrpc {

template <class EndPointT> class Message {
public:
private:
  EndPointT remote_addr_;
  StreamBuffer buffer_;
  ErrorCode status_;
};

};
