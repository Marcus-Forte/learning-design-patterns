

#include "event_bus_service.hh"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());
  EventBusServiceImpl service;
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on 0.0.0.0:50051\n";
  server->Wait();
}