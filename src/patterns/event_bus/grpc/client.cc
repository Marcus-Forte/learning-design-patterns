#include "event_bus.grpc.pb.h"

#include <google/protobuf/empty.pb.h>
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

static int publish(eb::EventBus::Stub &stub, const std::string &event,
                   const std::string &content) {
  eb::PublishRequest req;
  req.set_event(parse_event(event));
  req.mutable_message()->set_content(content);

  grpc::ClientContext ctx;
  google::protobuf::Empty empty;

  auto status = stub.Publish(&ctx, req, &empty);
  if (!status.ok()) {
    std::cerr << "Publish failed: " << status.error_message() << "\n";
    return 1;
  }

  std::cout << "Published to event " << event << ": " << content << "\n";
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0]
              << " <publish|subscribe> <A|B|C> [message]\n";
    return 1;
  }

  const std::string mode = argv[1];
  const std::string event = argv[2];

  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());
  auto stub = eb::EventBus::NewStub(channel);

  try {
    if (mode == "subscribe") {
      return subscribe(*stub, event);
    }

    if (mode == "publish") {
      if (argc < 4) {
        std::cerr << "Publish requires a message payload\n";
        return 1;
      }
      return publish(*stub, event, argv[3]);
    }

    std::cerr << "Unknown mode: " << mode << " (use publish or subscribe)\n";
    return 1;

  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }
}