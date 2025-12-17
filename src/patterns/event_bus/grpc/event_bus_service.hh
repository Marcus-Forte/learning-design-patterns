#pragma once

#include "event_bus.grpc.pb.h"
#include "event_bus.hh"

class EventBusServiceImpl : public eb::EventBus::Service {
public:
  EventBusServiceImpl() = default;

  ::grpc::Status Publish(::grpc::ServerContext *context,
                         const eb::PublishRequest *request,
                         ::google::protobuf::Empty *response) override;
  ::grpc::Status Subscribe(::grpc::ServerContext *context,
                           const eb::SubscribeRequest *request,
                           ::grpc::ServerWriter<eb::Message> *writer) override;

  ::grpc::Status
  getNumberOfSubscribers(::grpc::ServerContext *context,
                         const ::google::protobuf::Empty *request,
                         eb::NumberOfSubscribersResponse *response) override;

private:
  EventBus bus_;
};