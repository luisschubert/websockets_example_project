# Websockets Example Project

to run:
```bash
docker build -t websocket_example .
docker run -it websocket_example
```

To build and run the server Docker image:

```bash
docker build -f Dockerfile.server -t websocket_example_server .
docker run -d --name websocket_server -p 8080:8080 websocket_example_server
```

To build and run the client Docker image:
```bash
docker build -f Dockerfile.client -t websocket_example_client .
docker run -it --link websocket_server websocket_example_client
```


^ above is legacy.

better way is with docker compose:
added `docker-compose.yml`
```bash
docker-compose up
```
if you need to rebuild add `--build`.
and if you want the console back add `-d`.

to see logs for the containers:
`docker-compose logs -f server`
`docker-compose logs -f client`

and to bring them down:
`docker-compose down`

```
In the example, I'm assuming the JSON messages have the following structure:

HeartbeatPeriodRequest: {} (an empty object)
HeartbeatPeriodResponse: {"period": <number>}
HeartbeatRequest: {"client_id": "client_number_1"}
HeartbeatResponse: {} (an empty object)
```


