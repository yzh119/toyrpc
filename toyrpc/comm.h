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

template<class EndPointT>
class CommBase {
public:
  typedef Message<EndPointT> MessageType;
  typedef std::shared_ptr<MessageType> MessagePtr;

  CommBase(){};
  virtual ~CommBase(){};

  // starts polling.
  virtual void Start() = 0;
  // Send signal to worker thread and ask them to exit.
  virtual void SignalHandlerThreadsToExit() = 0;

  // Send message out, address is contained in the message.
  virtual CommErrors Send(const MessagePtr & msg) = 0;

  // Asynchronous send.
  // TODO(zihao): to figure out how it works.
  virtual void AsyncSend(const MessagePtr & msg,
                         const std::function<void(const MessagePtr& msg, CommErrors ec)>& callback) = 0;

  // Receives a message, the source of the message is contained in the message.
  virtual MessagePtr Recv() = 0;
};

template <class EndPointT>
inline const std::string EPToString(const EndPointT & ep) {
  return std::to_string(ep);
}

template <class EndPointT>
inline EndPointT MakeEP(const std::string& host, uint16_t port) {
  return EndPointT(host, port);
}

};
