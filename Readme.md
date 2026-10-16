# Http-Cerver

A lightweight HTTP server built from scratch in **C**, using low-level TCP sockets.

Http-Cerver is a personal systems-programming project focused on understanding how web servers work under the hood — from accepting TCP connections and parsing HTTP requests to handling methods and routing requests to the appropriate handlers.

The long-term goal is to evolve it into a small backend framework while keeping control over the underlying implementation.

## Features

- **TCP networking** — initialize sockets, bind to a port, listen for connections, and accept clients.
- **HTTP request parsing** — extract request information, including the HTTP method, route, and protocol version.
- **HTTP methods** — represent supported request methods using C enums.
- **Basic routing** — match incoming requests against registered routes and handle unmatched routes.
- **Custom class system** — integrate a custom object-oriented programming library implemented in C.
- **CMake build system** — organize the project and its compiled components into a maintainable build structure.

## Built With

- C
- TCP/IP sockets
- POSIX socket APIs on Linux
- CMake
- A custom C class library

## Getting Started

### Prerequisites

- A C compiler, such as GCC or Clang
- CMake 3.20 or later
- A compatible environment for the socket implementation

### Build

Clone the repository:

```bash
git clone https://github.com/YanisBelkacemi/Http-Cerver.git
cd Http-Cerver
```

Configure the project:

```bash
cmake -S . -B build
```

Compile the server:

```bash
cmake --build build
```

Run the generated executable from the build directory or the location configured by your CMake project.

> **Note:** The exact executable path depends on your CMake configuration. Windows and Linux may also require different socket implementations or build settings.

## How It Works

At a high level, the server follows a request-handling pipeline:

```text
Client
  |
  | HTTP request over TCP
  v
TCP Socket
  |
  v
Accept Connection
  |
  v
Parse HTTP Request
  |
  v
Extract Method and Route
  |
  v
Route Matching
  |
  v
Generate HTTP Response
  |
  v
Send Response to Client
```

Each stage is implemented in C, providing hands-on experience with networking, string handling, memory management, and the structure of HTTP communication.

## Project Structure

The project is organized around the server implementation and supporting libraries.

```text
Http-Cerver/
├── CMakeLists.txt
├── Http-Cerver.c
└── libs/
    └── classes/
        ├── headers/
        └── source/
```

*The structure above is illustrative; adjust it to match the current repository tree.*

## Goals and Roadmap

Http-Cerver is an evolving project. Planned improvements include:

- [x] Establish TCP socket communication.
- [x] Parse basic HTTP request information.
- [x] Represent HTTP methods using enums.
- [x] Implement initial route handling.
- [ ] Improve route registration and lookup.
- [ ] Introduce a cleaner request-handler architecture.
- [ ] Improve HTTP response construction and error handling.
- [ ] Serve HTML, CSS, JavaScript, and other static files.
- [ ] Explore database integration for dynamic backend applications.
- [ ] Improve resource management and connection handling.
- [ ] Expand testing and protocol compliance.

## What I'm Learning

This project is primarily about learning how software works below the abstraction level of established backend frameworks.

Some of the main concepts explored are:

- TCP sockets and network communication.
- HTTP request and response structure.
- C strings, buffers, and memory management.
- Data structures and route lookup.
- Modular architecture and reusable C libraries.
- Error handling in systems programming.
- Build configuration and linking with CMake.

One of the main motivations behind this project is to understand the mechanisms that backend frameworks normally hide and eventually use that knowledge to build a small framework of my own.

## Status

**Work in progress.**

Http-Cerver is an experimental learning project, not a production-ready web server. Protocol coverage, robustness, and security still need further development before it should be used to expose real applications to untrusted networks.

## Author

**Yanis Belkacemi**

- GitHub: [@YanisBelkacemi](https://github.com/YanisBelkacemi)

