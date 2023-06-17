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


here's the output of the two containers talking json to each other.

### docker compose output
```bash
lschubert@LUSN9994YRV74 websockets_example_project % docker-compose up --build -d
[+] Building 13.0s (19/19) FINISHED                                                                                                                                                                                                                  
 => [server internal] load build definition from Dockerfile.server                                                                                                                                                                              0.0s
 => => transferring dockerfile: 667B                                                                                                                                                                                                            0.0s
 => [server internal] load .dockerignore                                                                                                                                                                                                        0.0s
 => => transferring context: 2B                                                                                                                                                                                                                 0.0s
 => [client internal] load metadata for docker.io/library/ubuntu:20.04                                                                                                                                                                          1.0s
 => [client 1/7] FROM docker.io/library/ubuntu:20.04@sha256:f8f658407c35733471596f25fdb4ed748b80e545ab57e84efbdb1dbbb01bd70e                                                                                                                    0.0s
 => [server internal] load build context                                                                                                                                                                                                        0.0s
 => => transferring context: 9.25kB                                                                                                                                                                                                             0.0s
 => CACHED [client 2/7] RUN ln -snf /usr/share/zoneinfo/$CONTAINER_TIMEZONE /etc/localtime && echo $CONTAINER_TIMEZONE > /etc/timezone                                                                                                          0.0s
 => CACHED [client 3/7] RUN apt-get update && apt-get install -y     g++     cmake     libboost-all-dev     wget                                                                                                                                0.0s
 => CACHED [client 4/7] RUN wget -P /usr/include https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp                                                                                                                             0.0s
 => [server 5/7] COPY . /app                                                                                                                                                                                                                    0.0s
 => [server 6/7] WORKDIR /app                                                                                                                                                                                                                   0.0s
 => [server 7/7] RUN mkdir build && cd build && cmake .. && make websocket_example_server                                                                                                                                                       5.2s
 => [server] exporting to image                                                                                                                                                                                                                 0.0s
 => => exporting layers                                                                                                                                                                                                                         0.0s
 => => writing image sha256:cb2b1b589d6f4cae89a408387717bdc723d0c81c4fe06184cee9bd7e4d2807e7                                                                                                                                                    0.0s
 => => naming to docker.io/library/websockets_example_project-server                                                                                                                                                                            0.0s
 => [client internal] load .dockerignore                                                                                                                                                                                                        0.0s 
 => => transferring context: 2B                                                                                                                                                                                                                 0.0s 
 => [client internal] load build definition from Dockerfile.client                                                                                                                                                                              0.0s
 => => transferring dockerfile: 654B                                                                                                                                                                                                            0.0s
 => [client internal] load build context                                                                                                                                                                                                        0.0s
 => => transferring context: 6.93kB                                                                                                                                                                                                             0.0s
 => CACHED [client 5/7] COPY . /app                                                                                                                                                                                                             0.0s
 => CACHED [client 6/7] WORKDIR /app                                                                                                                                                                                                            0.0s
 => [client 7/7] RUN mkdir build && cd build && cmake .. && make websocket_example_client                                                                                                                                                       6.4s
 => [client] exporting to image                                                                                                                                                                                                                 0.0s
 => => exporting layers                                                                                                                                                                                                                         0.0s
 => => writing image sha256:c8f8fe5c5f04743a6be0b6bac402fff6b5a6708d044ca791e79b8462f57b1048                                                                                                                                                    0.0s
 => => naming to docker.io/library/websockets_example_project-client                                                                                                                                                                            0.0s
[+] Running 3/3                                                                                                                                                                                                                                      
 ✔ Network websockets_example_project_default     Created                                                                                                                                                                                       0.0s 
 ✔ Container websockets_example_project-server-1  Started                                                                                                                                                                                       0.3s 
 ✔ Container websockets_example_project-client-1  Started  
 ```

### client output
```bash
lschubert@LUSN9994YRV74 websockets_example_project % docker-compose logs -f client
websockets_example_project-client-1  | [Client] auto const results = resolver.resolve("localhost", "8080");
websockets_example_project-client-1  | [Client] connect with localhost
websockets_example_project-client-1  | [Client] Handshake with localhost
websockets_example_project-client-1  | [Client] Writing Heartbeat Period Request
websockets_example_project-client-1  | [Client] Wrote Heartbeat Period Request
websockets_example_project-client-1  | [Client] Got Heartbeat Period Response
websockets_example_project-client-1  | {"period":10}
websockets_example_project-client-1  | 0
websockets_example_project-client-1  | [Client] Sleeping for 10
websockets_example_project-client-1  | [Client] Done Sleeping
websockets_example_project-client-1  | [Client] Writing Heartbeat Request 
websockets_example_project-client-1  | [Client] Wrote Heartbeat Request
websockets_example_project-client-1  | [Client] Got Heartbeat Response
websockets_example_project-client-1  | [Client] Sleeping for 10
websockets_example_project-client-1  | [Client] Done Sleeping
websockets_example_project-client-1  | [Client] Writing Heartbeat Request 
websockets_example_project-client-1  | [Client] Wrote Heartbeat Request
websockets_example_project-client-1  | [Client] Got Heartbeat Response
websockets_example_project-client-1  | [Client] Sleeping for 10
websockets_example_project-client-1  | [Client] Done Sleeping
websockets_example_project-client-1  | [Client] Writing Heartbeat Request 
websockets_example_project-client-1  | [Client] Wrote Heartbeat Request
websockets_example_project-client-1  | [Client] Got Heartbeat Response
websockets_example_project-client-1  | [Client] Sleeping for 10
```

### server output
```bash
lschubert@LUSN9994YRV74 websockets_example_project % docker-compose logs -f server
websockets_example_project-server-1  | [Server] tcp::acceptor acceptor(ioc, {tcp::v4(), 8080});
websockets_example_project-server-1  | [Server] tcp::socket socket(ioc);
websockets_example_project-server-1  | [Server] acceptor.accept(socket)
websockets_example_project-server-1  | [Server] std::thread([socket = std::move(socket)]()
websockets_example_project-server-1  | [Server] tcp::socket socket(ioc);
websockets_example_project-server-1  | [Server] acceptor.accept(socket)
websockets_example_project-server-1  | [Server] websocket_stream ws(std::move(socket));
websockets_example_project-server-1  | [Server] ws.accept();
websockets_example_project-server-1  | [Server] ws.read(buffer);
websockets_example_project-server-1  | [Server] auto msg = json::parse(boost::beast::buffers_to_string(buffer.data()));
websockets_example_project-server-1  | [Server] write HeartbeatPeriodRequest
websockets_example_project-server-1  | [Server] ws.read(buffer);
websockets_example_project-server-1  | [Server] auto msg = json::parse(boost::beast::buffers_to_string(buffer.data()));
websockets_example_project-server-1  | [Server] write HeartbeatRequest
websockets_example_project-server-1  | [Server] ws.read(buffer);
websockets_example_project-server-1  | [Server] auto msg = json::parse(boost::beast::buffers_to_string(buffer.data()));
websockets_example_project-server-1  | [Server] write HeartbeatRequest
```



