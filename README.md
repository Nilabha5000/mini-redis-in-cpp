# Mini Redis Server (C++)

A **Redis-inspired in-memory key–value database** implemented in **C++** using **TCP sockets** and the **RESP (Redis Serialization Protocol)**.

This project is built for **learning backend systems**, focusing on:
- low-level networking
- protocol parsing
- command execution
- server–client communication

⚠️ This is **not a production-ready Redis replacement**.  
It is a **learning-oriented implementation** built step by step to understand how Redis-like systems work internally.

---

## Features

- TCP client–server architecture using `socket`, `bind`, `listen`, `accept`, `send`, `recv`
- RESP protocol **encoding (client)** and **decoding (server)**
- In-memory key–value storage using STL containers
- Command validation and proper RESP responses
- Error handling using RESP error format
- Automatic key expiration with TTL (EX) support
---

## Supported Commands

- | Command | Description | Response Type |
- |-------|-------------|--------------|
- | `SET key value` | Store a key–value pair | Simple String (`+OK`) |
- | `GET key` | Retrieve value of a key | Bulk String / Nil |
- | `DEL key` | Delete a key | Integer |
- | `EXISTS key` | Check if key exists | Integer |
- | `INCR key` | Increment integer value | Integer |
- | `DECR key` | Decrement integer value | Integer |
- | `PING [message]`|Returns PONG if no argument is provided, otherwise return a copy of the argument as a bulk | Bulk String / Simple String (`+PONG`) |

---

## RESP Protocol Support

### Client → Server (Encoding)
Commands are converted into RESP format before sending.

## Example:
# Encoded as:
```bash
*3\r\n
$3\r\nSET\r\n
$4\r\nname\r\n
$5\r\nAlice\r\n
```

### Server → Client (Responses)
- Simple Strings: `+OK\r\n`
- Errors: `-ERR message\r\n`
- Integers: `:1\r\n`
- Bulk Strings: `$5\r\nvalue\r\n`
- Nil: `$-1\r\n`

---

## Project Structure
```
├── client-cli.cpp        # Client CLI, RESP encoding/decoding
├── client-cli.h          # Client interface and macros
├── server.cpp            # TCP server loop, request handling
├── server.h              # Server class definition
├── database.cpp          # In-memory key–value store with TTL logic
├── database.h            # Database interface
├── command_dispatcher.h  # Command-to-handler mapping (SET, GET, DEL, etc.)
└── README.md              # Project overview and usage
```
---

## How It Works (High Level)

1. Client reads command from CLI
2. Client tokenizes input and encodes it using RESP
3. Server receives raw RESP string
4. Server decodes RESP into tokens
5. Command is executed on in-memory database
6. Server sends RESP-formatted response
7. Client decodes response and displays output

---

## Build & Run

### Compile
```bash
g++ server.cpp database.cpp -o server
g++ client-cli.cpp -o client
```
### Server Run
```bash
./server
```
### Client Run
```bash
./client-cli
```
### Example session

```bash
mini-redis> SET counter 10
OK

mini-redis> INCR counter
(integer) 11

mini-redis> GET counter
11

mini-redis> DEL counter
(integer) 1

mini-redis> GET counter
(nil)
mini-redis> SET counter1 100 EX 10 #expires after 10 seconds
OK
```

## Limitations
- Single client connection

- No persistence (in-memory only)

- No concurrency or event loop

- Assumes one recv() = one complete RESP message

- These are intentional trade-offs for learning clarity.

