#include "event_bus_service.hh"

::grpc::Status
EventBusServiceImpl::Publish(::grpc::ServerContext *context,
                             const eb::PublishRequest *request,
                             ::google::protobuf::Empty *response) {
  EventType event = static_cast<EventType>(request->event());
  bus_.publish(event, Message{request->message().content()});
  return ::grpc::Status::OK;
}

::grpc::Status
EventBusServiceImpl::Subscribe(::grpc::ServerContext *context,
                               const eb::SubscribeRequest *request,
                               ::grpc::ServerWriter<eb::Message> *writer) {

  // Implement your subscribe logic here

  Subscriber sub(bus_);
  EventType event = static_cast<EventType>(request->event());
  sub.subscribe(event, [&writer](const Message &msg) {
    eb::Message grpc_msg;

    grpc_msg.set_content(msg.payload);
    writer->Write(grpc_msg);
    // Handle the received message
  });

  while (!context->IsCancelled()) {
  }

  sub.unsubscribe(event);
  return ::grpc::Status::OK;
}

::grpc::Status EventBusServiceImpl::getNumberOfSubscribers(
    ::grpc::ServerContext *context, const ::google::protobuf::Empty *request,
    eb::NumberOfSubscribersResponse *response) {
  response->set_count(bus_.get_number_of_subscribers());
  return ::grpc::Status::OK;
}