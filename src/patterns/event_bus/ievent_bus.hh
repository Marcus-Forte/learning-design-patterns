#pragma once

#include <functional>
#include <string>

struct Message {
  std::string payload;
};

using Callback = std::function<void(const Message &)>;

enum class EventType { EVENT_TYPE_A = 0, EVENT_TYPE_B = 1, EVENT_TYPE_C = 2 };

class ISubscriber {
public:
  virtual void notify(const Message &msg) = 0;
};

class IPublisher {
public:
  virtual void publish(EventType type, const Message &msg) = 0;
};