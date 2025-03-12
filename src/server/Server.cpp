#include <algorithm>
#include <cerrno>
#include <cstring>
#include <unistd.h>

#include "logger/logging.hpp"
#include "server/Client.hpp"
#include "server/Server.hpp"
#include "utils.hpp"

bool Server::_exit = false;

Server::Server() {}

Server::~Server() {
  for (const auto &listener : _listeners) {
    listener.disconnect();
  }
  for (const auto &client : _clients) {
    client.disconnect();
  }
}

Server::Server(const Server &other) { *this = other; }

Server &Server::operator=(const Server &other) {
  if (this == &other)
    return *this;
  _listeners = other._listeners;
  _pollFd = other._pollFd;
  _clients = other._clients;
  return *this;
}

bool Server::addEntryPoint(const std::string &IPAddress, unsigned short port) {
  if (_existListenerSocket(IPAddress, port))
    return true;
  Socket serverSocket = Socket();
  if (!serverSocket.initialize()) {
    _printError("socket.initialize() failed");
    return false;
  }
  if (!_tryBindSocket(serverSocket, IPAddress, port)) {
    serverSocket.disconnect();
    return false;
  }
  _listeners.push_back(serverSocket);
  _pollFd.push_back(serverSocket.getPollFd(POLLIN));
  return true;
}

bool Server::listen() {
  logging::info("Start listening");
  for (size_t i = 0; i < _listeners.size(); i++) {
    if (!_tryListenSocket(_listeners[i])) {
      return false;
    }
    logging::debug(_listeners[i].getIPAndPort());
  }
  while (!Server::_exit) {
    if (poll(_pollFd.data(), _pollFd.size(), CLIENT_TIMEOUT_MS) == -1) {
      if (Server::_exit) {
        return true;
      }
      _printError("server.listen poll() failed");
      return false;
    }
    _handleSockets();
  }
  return true;
}

bool Server::_existListenerSocket(const std::string &IPAddress,
                                  unsigned short port) {
  for (const auto &listener : _listeners) {
    if ((listener.getIPAddress() == UNSPECIFIED_ADDRESS &&
         listener.getPort() == port) ||
        (listener.getIPAddress() == IPAddress && listener.getPort() == port)) {
      return true;
    }
  }
  return false;
}

bool Server::_tryBindSocket(Socket &socket, const std::string &IPAddress,
                            unsigned short port) {
  for (int i = 0; i < PERSISTENCE_TRIALS; i++) {
    if (socket.binding(IPAddress, port))
      return true;
    _printError("socket.binding() to " + IPAddress + ":" + toString(port) +
                " failed");
    ftSleep(PERSISTENCE_SLEEP_MS);
  }
  return false;
}

bool Server::_tryListenSocket(const Socket &socket) {
  for (int i = 0; i < PERSISTENCE_TRIALS; i++) {
    if (socket.listening())
      return true;
    _printError("socket.listening() to " + socket.getIPAndPort() + " failed");
    ftSleep(PERSISTENCE_SLEEP_MS);
  }
  return false;
}

void Server::_handleSockets() {
  for (size_t i = 0; i < _pollFd.size();) {
    bool isDisconnect = false;
    if (i >= _listeners.size()) {
      isDisconnect = !_handleClient(_pollFd[i], i - _listeners.size());
    } else if ((_pollFd[i].revents & POLLIN) == 1 && i < _listeners.size()) {
      _acceptNewClient(_pollFd[i]);
    }
    if (isDisconnect) {
      _pollFd.erase(_pollFd.begin() + i);
      logging::debug("A client has been disconnected");
    } else
      i++;
  }
}

bool Server::_acceptNewClient(struct pollfd listener) {
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;
  socklen_t serverAddressLength = sizeof(serverAddress);
  socklen_t clientAddressLength = sizeof(clientAddressLength);

  int clientSocket = accept(listener.fd, (struct sockaddr *)&clientAddress,
                            &clientAddressLength);
  if (clientSocket == -1) {
    _printError("server.listen accept() failed");
    return false;
  }
  if (getsockname(clientSocket, (struct sockaddr *)&serverAddress,
                  &serverAddressLength) == -1) {
    _printError("server.listen getsockname() failed");
    close(clientSocket);
    return false;
  }
  if (_clients.size() < MAX_CLIENT) {
    Client client(clientSocket, clientAddress, serverAddress);
    _clients.push_back(client);
    _pollFd.push_back(
        (struct pollfd){.fd = clientSocket, .events = POLLIN, .revents = 0});
    logging::debug("A new client is connected");
  } else {
    logging::warning("Too many clients connected");
    close(clientSocket);
  }
  return true;
}

bool Server::_handleClient(const struct pollfd &pollSocket,
                           size_t clientIndex) {
  std::vector<Client>::iterator clientIt = _clients.begin() + clientIndex;
  Client &client = *clientIt;
  if ((pollSocket.revents & POLLIN) == 0)
    return true;
  if (!_readClientRequest(client)) {
    _disconnectClient(clientIt);
    return false;
  }
  client.parseCommands();
  std::vector<std::string> commands = client.getCommands();
  for (const auto &command : commands) {
    if (command == "quit") {
      logging::info("Request quit");
      Server::stop();
    } else {
      logging::info("User input: " + command);
    }
  }
  client.clearCommands();
  return true;
}

bool Server::_readClientRequest(Client &client) {
  char buffer[READ_BUFFER_SIZE];
  std::string strRequest;

  ssize_t bytesRead = recv(client.getFD(), buffer, sizeof(buffer), 0);
  if (bytesRead == -1)
    return true;
  if (bytesRead == 0)
    return false;
  strRequest.append(buffer, bytesRead);
  client.appendRawRequest(strRequest);
  return true;
}

void Server::_disconnectClient(std::vector<Client>::iterator clientIt) {
  if (clientIt == _clients.end())
    return;
  clientIt->disconnect();
  _clients.erase(clientIt);
}

void Server::_printError(const std::string &error) const {
  logging::error(error + " (" + toString(errno) + ": " + strerror(errno) + ")");
}

void Server::stop() {
  logging::info("Stopping server");
  Server::_exit = true;
}
