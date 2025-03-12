#ifndef SERVER_CLIENT_HPP
#define SERVER_CLIENT_HPP

#include <arpa/inet.h>
#include <vector>

#include "server/Socket.hpp"

class Client {
public:
  Client();
  Client(int fd, struct sockaddr_in address, struct sockaddr_in entryAddress);
  Client(const Client &other);
  ~Client();

  Client &operator=(const Client &other);

  void appendRawRequest(const std::string &rawRequest);
  void setRawRequest(const std::string &rawRequest);
  int getFD();
  const std::string &getEntryIPAddress() const;
  unsigned short getEntryPort() const;
  int disconnect() const;

  void parseCommands();
  const std::vector<std::string> &getCommands() const;
  void clearCommands();

private:
  Socket _socket;
  std::string _rawRequest;
  std::string _entryIPAddress;
  struct sockaddr_in _entryAddress;
  unsigned short _entryPort;
  std::vector<std::string> _commands;
};

#endif
