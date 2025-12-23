# Generate proto.

From root of repo, run:

```bash
uv run python -m grpc_tools.protoc -I src/patterns/event_bus/grpc --python_out=py/patterns/event_bus --grpc_python_out=py/patterns/event_bus src/patterns/event_bus/grpc/event_bus.proto
```