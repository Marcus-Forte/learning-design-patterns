#include "event_bus.hh"
#include <iostream>

int main(int argc, char **argv) {

  EventBus bus;

  Subscriber aa(bus);
  Subscriber ab(bus);

  aa.subscribe(EventType::EVENT_TYPE_A, [&](const Message &msg) {
    std::cout << "Subscriber AA received: " << msg.payload << std::endl;
    bus.publish(EventType::EVENT_TYPE_B, Message{"Event B from AA"});
  });

  ab.subscribe(EventType::EVENT_TYPE_B, [](const Message &msg) {
    std::cout << "Subscriber AB received: " << msg.payload << std::endl;
  });

  aa.subscribe(EventType::EVENT_TYPE_A, [&](const Message &msg) {
    std::cout << " -- " << msg.payload << std::endl;
  });

  bus.publish(EventType::EVENT_TYPE_A, Message{"Event A"});
}