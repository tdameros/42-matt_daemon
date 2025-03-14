#ifndef DAEMON_DAEMON_HPP
#define DAEMON_DAEMON_HPP

#include <string>
#include <unistd.h>

#define DAEMON_LOCK_FOLDER "/var/lock"

class Daemon {
public:
  Daemon();
  Daemon(const std::string &name);
  Daemon(const Daemon &other);
  virtual ~Daemon();
  Daemon &operator=(const Daemon &other);

  bool daemonize();
  virtual void run() = 0;
  virtual void stop() = 0;

private:
  void _catchSignals();
  bool _tryLockFile();
  bool _removeLockFile();
  std::string _getLockFilePath();

  static void _setInstance(Daemon *instance);
  static void _signalHandler(int signum);

  static Daemon *_instance;

  std::string _name;
  pid_t _pid;
  int _lockFileFD;
  bool _hasLockFile;
};

#endif