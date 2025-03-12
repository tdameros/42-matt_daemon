#include "server/Client.hpp"
#include "server/Socket.hpp"

#include "utils.hpp"
#include <algorithm>
#include <ctime>
#include <string>

Client::Client() {}

Client::Client(int fd, struct sockaddr_in address,
               struct sockaddr_in entryAddress) {
  _socket = Socket(fd, address);
  _entryAddress = entryAddress;
  _entryIPAddress = Socket::networkToStr(entryAddress.sin_addr.s_addr);
  _entryPort = ntohs(entryAddress.sin_port);
}

Client::Client(const Client &other) { *this = other; }

Client::~Client() {}

Client &Client::operator=(const Client &other) {
  if (this == &other)
    return *this;
  _socket = other._socket;
  _entryAddress = other._entryAddress;
  _entryIPAddress = other._entryIPAddress;
  _entryPort = other._entryPort;
  return *this;
}

void Client::appendRawRequest(const std::string &rawRequest) {
  _rawRequest.append(rawRequest);
}

void Client::setRawRequest(const std::string &rawRequest) {
  _rawRequest = rawRequest;
}

int Client::getFD() { return _socket.getFD(); }

const std::string &Client::getEntryIPAddress() const { return _entryIPAddress; }

unsigned short Client::getEntryPort() const { return _entryPort; }

int Client::disconnect() const { return _socket.disconnect(); }

void Client::parseCommands() {
  std::vector<std::string> split_commands = splitDelimiter(_rawRequest, '\n');
  for (size_t i = 0; i < split_commands.size(); i++) {
    if (i == split_commands.size() - 1 &&
        _rawRequest[_rawRequest.size() - 1] != '\n') {
      _rawRequest = split_commands[i];
      break;
    } else {
      _rawRequest = "";
    }
    _commands.push_back(trim(split_commands[i]));
  }
}

const std::vector<std::string> &Client::getCommands() const {
  return _commands;
}

void Client::clearCommands() { _commands.clear(); }