#include "event_bus.grpc.pb.h"

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

static eb::EventType parse_event(const std::string &s) {
  if (s == "A" || s == "EVENT_A")
    return eb::EVENT_A;
  if (s == "B" || s == "EVENT_B")
    return eb::EVENT_B;
  if (s == "C" || s == "EVENT_C")
    return eb::EVENT_C;
  throw std::invalid_argument("Unknown event type: " + s);
}

static int subscribe(eb::EventBus::Stub &stub, const std::string &event) {
  eb::SubscribeRequest req;
  req.set_event(parse_event(event));

  grpc::ClientContext ctx;
  std::unique_ptr<grpc::ClientReader<eb::Message>> reader(
      stub.Subscribe(&ctx, req));

  eb::Message msg;
  while (reader->Read(&msg)) {
    std::cout << "Received message on event " << event << ": " << msg.content()
              << "\n";
  }

  auto status = reader->Finish();
  if (!status.ok()) {
    std::cerr << "Subscribe stream ended with error: " << status.error_message()
              << "\n";
    return 1;
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <A|B|C>\n";
    return 1;
  }

  const std::string event = argv[1];

  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());
  auto stub = eb::EventBus::NewStub(channel);

  try {
    return subscribe(*stub, event);

  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }
}