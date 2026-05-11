# TCP Thread Pool Server-Client Communication

基于 C++ 实现的简单 TCP 网络通信项目，采用 **客户端 / 服务端（C/S）架构**，结合 **线程池模型** 实现并发任务处理。

该项目主要用于学习和实践 C++ 网络编程、多线程并发编程以及线程池设计。

---

## Project Introduction

本项目包含独立客户端与服务端两个模块。

服务端负责：

- 监听客户端连接
- 接收客户端请求
- 将任务加入线程池处理
- 管理客户端连接生命周期

客户端负责：

- 建立 TCP 连接
- 向服务端发送消息
- 接收服务端响应

项目目标：

- 熟悉 TCP Socket 通信流程
- 理解客户端 / 服务端架构设计
- 实现线程池并发处理模型
- 提高网络编程实践能力

---

## Tech Stack

- C++
- WinSock2
- TCP Socket
- Windows Multithreading
- Thread Pool
- Visual Studio

---

## Project Structure

```bash
TCP-Thread-Pool
├── CS-0/    # Server
└── CS-1/    # Client
```

---

## Features

### Server

- TCP socket server
- Client connection listening
- Request receiving
- Thread pool task dispatching
- Concurrent request processing
- Connection resource management

### Client

- TCP server connection
- Message sending
- Response receiving
- Basic communication testing

---

## Core Implementation

### 1. TCP Network Communication

基于 WinSock API 实现完整 TCP 通信流程：

- socket()
- bind()
- listen()
- accept()
- connect()
- send()
- recv()
- closesocket()

实现客户端与服务端基础通信。

---

### 2. Thread Pool Design

服务端采用线程池处理任务：

- 预创建工作线程
- 任务队列管理
- 线程等待与唤醒机制
- 并发任务执行
- 降低频繁创建线程的开销

---

### 3. C/S Architecture

项目采用客户端 / 服务端分离设计：

- Server 负责连接管理与任务处理
- Client 负责请求发送与结果接收

实现基础网络服务架构。

---

## Development Environment

- Windows 10 / 11
- Visual Studio 2019 / 2022
- WinSock2

---

## Build & Run

### Start Server

进入：

```bash
CS-0
```

编译并运行服务端。

---

### Start Client

进入：

```bash
CS-1
```

编译并运行客户端。

---

### Test Steps

1. 启动服务端
2. 启动客户端
3. 建立 TCP 连接
4. 发送测试消息
5. 验证线程池任务处理

---

## Learning Outcomes

通过该项目实践了：

- TCP 网络编程
- Socket API 使用
- C++ 多线程编程
- 线程池实现原理
- 基础并发任务处理
- 客户端 / 服务端架构设计

---

## Future Improvements

- 支持多个客户端同时连接
- 使用 IO 多路复用优化连接管理
- 添加日志系统
- 优化线程池任务调度策略
- 增加异常连接处理机制

---

## Author

Kira
