# Mini HTTP Server

This is a simple multithreaded HTTP server written in C++ using `std::thread` and low-level file and socket handling. It supports:

- Handling `GET` requests to serve static files
- Handling `POST` requests to save uploaded data  
- Echo endpoint for testing
- User-agent detection
- File serving with custom directory support
- Listening on a specified port (default: `1337`)

## 🛠 Build

Make sure you have `g++` installed, then run:

```bash
make
```

Then start the server:

```bash
./http_server
```

For file serving with a custom directory:

```bash
./http_server --directory /path/to/your/files
```

## 🧪 Testing

Open a new terminal and test with the following commands:

### Echo Endpoint
```bash
curl -i localhost:1337/echo/hello
# You should see "hello" in the response body
```

### User-Agent Detection
```bash
curl -i --header "User-Agent: TestAgent" localhost:1337/user-agent
# You should see your user agent string in the response body
```

### File Operations
```bash
# GET a file (make sure test.txt exists in your directory)
curl -i localhost:1337/files/test.txt
# You should see the content of test.txt in the response body

# POST to create/update a file
curl -i localhost:1337/files/test.txt -X POST -d "Hello, World!"
# This will create or update test.txt with "Hello, World!"
```

## 📁 Project Structure

```
src/
├── main.cpp           # Entry point
├── server.cpp         # Server implementation
├── server.hpp         # Server header
├── resposeHandle.cpp  # Request/response handling
├── utils.cpp          # Utility functions
├── Makefile          # Build configuration
└── test.txt          # Sample file for testing
```
## ✨ Don't forget to visit the full implementation of the real [42Webserv](https://github.com/Lc0d3r/1337-Webserv.git) Repo.
  ## And you can see my [parsing implementation](https://github.com/abd3l3li/webserv_parsing_practice.git) of Webserv as well.
