# webServ

This is a custom HTTP server implementation that adheres to a specific set of requirements and features. The server is designed to be efficient, non-blocking, and capable of serving static websites, handling various HTTP methods, and processing CGI scripts. The server uses a configuration file to define its behavior and settings.
---

## Features
- Supports HTTP 1.1 protocol.
- Non-blocking.
- Provides default error pages .
- Handles fully static websites and serves static files efficiently.
- Supports multiple ports and hosts for different server configurations.
- Offers the ability to set-up routes with various rules and configurations.
- Accepts GET, POST, and DELETE methods.
- Supports 301 and 302 redirections, -directory listing, default file for 	directories, and CGI execution (python and php-CGI).
- Enables file uploads and allows configuring the upload destination.
- Support cookies and session management.
---

## Installation
1. Clone the repository:

```bash
git clone https://github.com/hasabir/WebServ.git
cd WebServ
```
2. Build the server:

```go
make
---
## Usage
- Run the server:

```bash
./webserv
```

---
## Configuration File

The configuration file config.conf is used to customize the server's behavior. Below are the available options:

**port:** Define the port on which the server should listen.

**host:** Define the host address for the server.

**server_names:** Set server names (optional).

**error_pages:** Specify the path to custom error pages (optional).

**client_body_limit:** Set the limit for client request body size.

---
# Additional configurations

1. Route Configurations

	Directives are defined in the configuration file and determine how the server handles specific URLs. Below are the supported route configurations:

**allow:** Define a list of accepted HTTP methods for the route.

**redirect:** Define a redirection URL for the route.

**root:** Set the directory or file from where the file should be searched.

**autoindex:** Turn on or off autoindex of the directory.

**index:** Set a default file to answer if the request is a directory.

**cgi:** Set up cgi extention file script and cgi exutable file path.

**upload:** Turn on or to support upload in specific location (optional).

**upload_store:** if upload is **on** setup file uploads and configure the upload destination.

---
# Testing

	To test the server, you can use the provided configuration files and default basic files:

Start the server:

```bash
./webserv
```
	Access the server from your web browser, or use tools like curl or to test various routes and features.

Ensure that all the required features, such as static file serving, route handling, and CGI execution, work as expected.

#### Programing Languages:
<p align="left">
	
<a href="https://en.cppreference.com/w/" target="_blank" rel="noreferrer"><img src="https://raw.githubusercontent.com/isocpp/logos/master/cpp_logo.png" width="36" height="36" alt="Cpp" /></a>
  <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/python/python-original.svg" alt="python" width="40" height="40"/>
  <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/html5/html5-original.svg" alt="HTML" width="40" height="40"/>
  <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/css3/css3-original.svg" alt="CSS" width="40" height="40"/>
  <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/php/php-original.svg" alt="PHP" width="40" height="40"/>
</p>






