#include "daemon/MattDaemon.hpp"
#include "logger/logging.hpp"

#define LOG_FILE "/var/log/matt_daemon.log"

int main() {
  logging::setEnableStdout(false);
  logging::setHasColor(true);
  logging::setHasDatetime(true);
  logging::setLevel(Tintin_reporter::DEBUG);
  logging::setFile(LOG_FILE);
  MattDaemon daemon;

  if (!daemon.daemonize()) {
    return 1;
  }
  return 0;
}
