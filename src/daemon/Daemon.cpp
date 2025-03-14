#include "daemon/Daemon.hpp"

#include <csignal>
#include <cstdio>
#include <iostream>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

#include "logger/logging.hpp"

Daemon *Daemon::_instance = nullptr;

Daemon::Daemon() : _name("daemon") {}

Daemon::Daemon(const std::string &name) : _name(name) {}

Daemon::Daemon(const Daemon &other) { *this = other; }

Daemon::~Daemon() {
  if (_lockFileFD != -1) {
    this->_removeLockFile();
    logging::info("Daemon stopped");
  }
}

Daemon &Daemon::operator=(const Daemon &other) {
  if (this == &other) {
    return *this;
  }
  _instance = other._instance;
  _pid = other._pid;
  return *this;
}

bool Daemon::daemonize() {
  if (geteuid() != 0) {
    std::cerr << "You must be root to start a daemon" << std::endl;
    return false;
  }
  if (!this->_tryLockFile()) {
    std::cerr << "A daemon is already started" << std::endl;
    return false;
  }
  logging::info("Start daemon");
  pid_t pid = fork();
  if (pid < 0) {
    logging::error("Fork failed");
    std::exit(EXIT_FAILURE);
  }
  if (pid > 0) {
    std::exit(EXIT_SUCCESS);
  }
  chdir("/");
  umask(0);
  setsid();
  this->_pid = getpid();

  freopen("/dev/null", "r", stdin);
  freopen("/dev/null", "w", stdout);
  freopen("/dev/null", "w", stderr);
  this->_catchSignals();
  logging::info("Daemon started");
  this->run();
  return true;
}

void Daemon::_catchSignals() {
  Daemon::_setInstance(this);
  std::signal(SIGTERM, Daemon::_signalHandler);
  std::signal(SIGINT, Daemon::_signalHandler);
  std::signal(SIGHUP, Daemon::_signalHandler);
  std::signal(SIGQUIT, Daemon::_signalHandler);
  std::signal(SIGSTOP, Daemon::_signalHandler);
  std::signal(SIGUSR1, Daemon::_signalHandler);
  std::signal(SIGUSR2, Daemon::_signalHandler);
}

void Daemon::_setInstance(Daemon *instance) { _instance = instance; }

void Daemon::_signalHandler(int signum) {
  static_cast<void>(signum);
  logging::info("Signal handler (" + std::to_string(signum) + ")");
  if (nullptr != _instance) {
    _instance->stop();
  }
}

bool Daemon::_tryLockFile() {
  int fd = open(this->_getLockFilePath().c_str(), O_RDWR | O_CREAT);
  if (fd == -1) {
    std::cerr << "open(" + this->_getLockFilePath() + ") failed" << std::endl;
    logging::error("failed to lock file");
    return false;
  }
  if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
    close(fd);
    std::cerr << "lockf(" + this->_getLockFilePath() + ") failed" << std::endl;
    logging::error("failed to lock file");
    return false;
  }
  _lockFileFD = fd;
  return true;
}

bool Daemon::_removeLockFile() {
  if (flock(_lockFileFD, LOCK_UN) == -1) {
    logging::error("lockf() failed" + std::string(strerror(errno)));
    close(_lockFileFD);
    return false;
  }
  close(_lockFileFD);
  if (std::remove(_getLockFilePath().c_str()) < 0) {
    logging::error(strerror(errno));
  }
  return true;
}

std::string Daemon::_getLockFilePath() {
  return std::string(DAEMON_LOCK_FOLDER) + "/" + _name + ".lock";
}