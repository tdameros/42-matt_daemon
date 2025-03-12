
#include "daemon/MattDaemon.hpp"
#include "logger/logging.hpp"

MattDaemon::MattDaemon() : Daemon("matt_daemon") {}

MattDaemon::MattDaemon(const MattDaemon &other) : Daemon(other) {
  *this = other;
}

MattDaemon::~MattDaemon() {}

MattDaemon &MattDaemon::operator=(const MattDaemon &other) {
  if (this == &other) {
    return *this;
  }
  return *this;
}

void MattDaemon::run() {
  Server server;
  if (!server.addEntryPoint("0.0.0.0", 4242)) {
    logging::error("server.addEntryPoint() failed");
    return;
  }
  server.listen();
}

void MattDaemon::stop() { Server::stop(); }