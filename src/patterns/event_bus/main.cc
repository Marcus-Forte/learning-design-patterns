#include "event_bus.hh"
#include <iostream>

int main(int argc, char **argv) {

  EventBus bus;

  auto subA = bus.subscribe(EventType::EVENT_TYPE_A, [&](const Message &msg) {
    std::cout << "Subscriber AA received: " << msg.payload << std::endl;
    bus.publish(EventType::EVENT_TYPE_B, Message{"Event B from AA"});
  });

  auto subB = bus.subscribe(EventType::EVENT_TYPE_B, [](const Message &msg) {
    std::cout << "Subscriber AB received: " << msg.payload << std::endl;
  });

  bus.subscribe(EventType::EVENT_TYPE_A, [&](const Message &msg) {
    std::cout << " -- " << msg.payload << std::endl;
  });

  bus.publish(EventType::EVENT_TYPE_A, Message{"Event A"});
}