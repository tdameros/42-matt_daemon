#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include <sys/poll.h>
#include <vector>

#include "server/Client.hpp"
#include "server/Socket.hpp"

#define PERSISTENCE_TRIALS 5
#define PERSISTENCE_SLEEP_MS 500

#define CLIENT_TIMEOUT_MS 10000

#define READ_BUFFER_SIZE 1024

#define MAX_CLIENT 3

class Server {
public:
  Server();
  Server(const Server &other);
  virtual ~Server();
  Server &operator=(const Server &other);

  bool addEntryPoint(const std::string &IPAddress, unsigned short port);
  bool listen();

  static void stop();

private:
  bool _existListenerSocket(const std::string &IPAddress, unsigned short port);
  bool _tryBindSocket(Socket &socket, const std::string &IPAddress,
                      unsigned short port);
  bool _tryListenSocket(const Socket &socket);
  bool _acceptNewClient(struct pollfd listener);
  void _handleSockets();
  bool _handleClient(const struct pollfd &pollSocket, size_t clientIndex);
  bool _readClientRequest(Client &client);
  void _disconnectClient(std::vector<Client>::iterator clientIt);

  void _printError(const std::string &error) const;

  static bool _exit;

  std::vector<Socket> _listeners;
  std::vector<struct pollfd> _pollFd;
  std::vector<Client> _clients;
};

#endif