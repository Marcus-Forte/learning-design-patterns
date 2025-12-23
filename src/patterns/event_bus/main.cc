#include "event_bus.hh"
#include <iostream>

int main(int argc, char **argv) {

  // Bus Object
  EventBus bus;

  // Subs
  auto subA = bus.subscribe(EventType::EVENT_TYPE_A, [&](const Message &msg) {
    std::cout << "Subscriber subA received: " << msg.payload << std::endl;
    // We can publish from within a subscriber (Be careful of infinite loops)
    bus.publish(EventType::EVENT_TYPE_B, Message{"Event B from within subA!"});
  });

  auto subB = bus.subscribe(EventType::EVENT_TYPE_B, [](const Message &msg) {
    std::cout << "Subscriber subB received: " << msg.payload << std::endl;
  });

  auto subC = bus.subscribe(EventType::EVENT_TYPE_A, [&](const Message &msg) {
    std::cout << "Subscriber subC received: " << msg.payload << std::endl;
  });

  // Publisher
  bus.publish(EventType::EVENT_TYPE_A, Message{"Event A"});
}