#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct Message {
  std::string payload;
};

enum class EventType { EVENT_TYPE_A = 0, EVENT_TYPE_B = 1, EVENT_TYPE_C = 2 };
using Callback = std::function<void(const Message &)>;

class EventBus {
public:
  struct Subscriber; // Forward declaration

  void publish(EventType type, const Message &msg) {
    for (const auto &sub : subscribers_[type]) {
      sub->callback(msg);
    }
  };

  std::shared_ptr<Subscriber> subscribe(EventType type, Callback cb) {

    auto sub =
        std::make_shared<Subscriber>(next_id_++, type, std::move(cb), this);
    subscribers_[type].insert(sub);
    return sub;
  };

  void unsubscribe(std::shared_ptr<Subscriber> sub) {
    subscribers_[sub->type].erase(sub);
  };

  size_t get_number_of_subscribers() const {
    size_t count = 0;
    for (const auto &pair : subscribers_) {
      count += pair.second.size();
    }
    return count;
  };

private:
  std::unordered_map<EventType, std::unordered_set<std::shared_ptr<Subscriber>>>
      subscribers_;
  size_t next_id_ = 0;

public:
  struct Subscriber {
    size_t id;
    EventType type;
    Callback callback;

    Subscriber(size_t sub_id, EventType sub_type, Callback cb, EventBus *bus)
        : id(sub_id), type(sub_type), callback(std::move(cb)), bus_(bus) {}

    /// \todo Implement RAII unsubscription

    ~Subscriber() {
      if (bus_) {
      }
    }

  private:
    EventBus *bus_;
  };
};
