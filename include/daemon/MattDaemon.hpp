
#ifndef DAEMON_MATTDAEMON_HPP
#define DAEMON_MATTDAEMON_HPP

#include "Daemon.hpp"
#include "server/Server.hpp"

class MattDaemon : public Daemon {
public:
  MattDaemon();
  MattDaemon(const MattDaemon &other);
  virtual ~MattDaemon();
  MattDaemon &operator=(const MattDaemon &other);

  void run() override;
  void stop() override;

private:
  Server _server;
};

#endif