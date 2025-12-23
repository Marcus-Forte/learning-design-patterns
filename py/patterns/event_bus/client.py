import argparse
import sys

import grpc
from google.protobuf import empty_pb2

import event_bus_pb2 as pb2
from event_bus_pb2_grpc import EventBusStub


EVENT_ALIASES = {
    "A": pb2.EVENT_A,
    "EVENT_A": pb2.EVENT_A,
    "B": pb2.EVENT_B,
    "EVENT_B": pb2.EVENT_B,
    "C": pb2.EVENT_C,
    "EVENT_C": pb2.EVENT_C,
}


def parse_event(text: str) -> pb2.EventType:
    try:
        return EVENT_ALIASES[text.strip().upper()]
    except KeyError as exc:
        msg = "Unknown event '%s'. Use A, B, or C." % text
        raise argparse.ArgumentTypeError(msg) from exc


def publish(stub: EventBusStub, event: pb2.EventType, message: str) -> int:
    req = pb2.PublishRequest(event=event, message=pb2.Message(content=message))
    try:
        stub.Publish(req)
    except grpc.RpcError as exc:
        sys.stderr.write("Publish failed: %s\n" % (exc.details() or exc))
        return 1

    event_name = pb2.EventType.Name(event)
    print("Published to %s: %s" % (event_name, message))
    return 0


def subscribe(stub: EventBusStub, event: pb2.EventType) -> int:
    req = pb2.SubscribeRequest(event=event)
    event_name = pb2.EventType.Name(event)
    try:
        for msg in stub.Subscribe(req):
            # Stream blocks until the server yields or the channel closes.
            print("[%s] %s" % (event_name, msg.content))
    except KeyboardInterrupt:
        return 0
    except grpc.RpcError as exc:
        if exc.code() == grpc.StatusCode.CANCELLED:
            return 0
        sys.stderr.write("Subscribe failed: %s\n" % (exc.details() or exc))
        return 1
    return 0


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="EventBus Python client (gRPC)")
    parser.add_argument(
        "--target",
        default="localhost:50051",
        help="host:port of EventBus server (default: %(default)s)",
    )

    sub = parser.add_subparsers(dest="command", required=True)

    pub = sub.add_parser("publish", help="Publish a message to an event")
    pub.add_argument("event", type=parse_event, help="Event to publish (A, B, C)")
    pub.add_argument("message", help="Message content")

    sub_sub = sub.add_parser("subscribe", help="Subscribe to an event stream")
    sub_sub.add_argument(
        "event", type=parse_event, help="Event to subscribe to (A, B, C)"
    )

    sub.add_parser("count", help="Get number of active subscribers")

    return parser


def main(argv: list[str] | None = None) -> int:
    args = build_parser().parse_args(argv)

    with grpc.insecure_channel(args.target) as channel:
        stub = EventBusStub(channel)

        match args.command:
            case "publish":
                return publish(stub, args.event, args.message)
            case "subscribe":
                return subscribe(stub, args.event)
            case _:  # pragma: no cover - argparse enforces choices
                return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())

