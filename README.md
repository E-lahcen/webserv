# Webserv

## Overview
**Webserv** is a project that helps you dive deep into the inner workings of HTTP and understand why a URL starts with `http://`. The goal is to create your own HTTP server in **C++98**, which can be tested with an actual browser. This project will enhance your understanding of HTTP, one of the most widely used protocols on the internet.

---

## Summary
- **Version**: 21  
- **Purpose**: Write an HTTP server in C++98.  
- **Main Features**:
  - Serve static websites.
  - Support `GET`, `POST`, and `DELETE` methods.
  - Allow file uploads.
  - Provide custom configuration files.
- **Bonus Features**:
  - Cookie and session management.
  - Multiple CGI support.

---

## Contents
1. [Introduction](#introduction)  
2. [General Rules](#general-rules)  
3. [Mandatory Part](#mandatory-part)  
   - [Requirements](#requirements)  
   - [For MacOS Only](#for-macos-only)  
   - [Configuration File](#configuration-file)  
4. [Bonus Part](#bonus-part)  
5. [Submission and Peer-Evaluation](#submission-and-peer-evaluation)  

---

## Introduction
HTTP (Hypertext Transfer Protocol) is the foundation of data communication for the World Wide Web. It enables:
- Delivery of hypertext documents with hyperlinks.
- Interaction between clients and servers for resource requests.
- File uploads and form submissions.

Web servers process and deliver these resources, often storing them as HTML documents. High-traffic websites may utilize multiple servers for scalability.

---

## General Rules
- Your program **must not crash** under any circumstances.  
- Provide a `Makefile` with at least these rules: `$(NAME)`, `all`, `clean`, `fclean`, `re`.  
- Use `c++` with `-Wall -Wextra -Werror` flags.  
- Code must comply with the **C++98 standard**.  
- External libraries, including Boost, are forbidden.  

---

## Mandatory Part

### Program Information
- **Program Name**: `webserv`  
- **Turn-in Files**:  
  - `Makefile`  
  - Source files (`*.h`, `*.hpp`, `*.cpp`, etc.)  
  - Configuration files  
- **Compilation Flags**: `-Wall -Wextra -Werror -std=c++98`  

### Requirements
- Use **poll()** (or equivalent) for all I/O operations.  
- Clients should never hang or block indefinitely.  
- Support:
  - Multiple ports.
  - Default error pages.
  - File uploads.
  - `GET`, `POST`, `DELETE` methods.  
- Configuration file options:
  - Set ports and hosts.
  - Define routes with accepted HTTP methods.
  - Enable CGI execution.
  - Set file upload paths.  

### For MacOS Only
MacOS requires non-blocking file descriptors using `fcntl()`:
```cpp
fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
