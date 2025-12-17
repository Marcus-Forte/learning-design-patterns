#pragma once

#include "ievent_bus.hh"
#include <unordered_set>

class EventBus {
public:
  void publish(EventType type, const Message &msg) {
    for (const auto &sub : subscribers_[type]) {
      sub->notify(msg);
    }
  };

  unsigned int get_number_of_subscribers() const {
    unsigned int total = 0;
    for (const auto &pair : subscribers_) {
      total += pair.second.size();
    }
    return total;
  };

private:
  std::unordered_map<EventType, std::unordered_set<ISubscriber *>> subscribers_;

  void add_subscriber(EventType type, ISubscriber *cb) {
    subscribers_[type].insert(cb);
  };

  void remove_subscriber(EventType type, ISubscriber *cb) {
    subscribers_[type].erase(cb);
  };

  friend class Subscriber; // Subs can call add_subscriber / remove_subscriber.
};

// Concrete implementations

class Subscriber : public ISubscriber {
public:
  Subscriber(EventBus &bus) : bus_(bus) {};

  void subscribe(EventType type, Callback cb) {
    bus_.add_subscriber(type, this);
    cb_ = std::move(cb);
  };

  void unsubscribe(EventType type) { bus_.remove_subscriber(type, this); };

  void notify(const Message &msg) { cb_(msg); }

private:
  EventBus &bus_;
  Callback cb_;
};
