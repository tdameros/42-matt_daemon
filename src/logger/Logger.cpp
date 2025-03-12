#include "logger/Tintin_reporter.hpp"

#include <ctime>
#include <fstream>
#include <iostream>

Tintin_reporter::Tintin_reporter()
    : _file(""), _hasFile(false), _hasColor(false), _hasDatetime(false),
      _enableStdout(true), _level(WARNING) {}

Tintin_reporter::Tintin_reporter(const Tintin_reporter &other) {
  *this = other;
}

Tintin_reporter::~Tintin_reporter() {}

Tintin_reporter &Tintin_reporter::operator=(const Tintin_reporter &other) {
  if (this == &other)
    return *this;
  _file = other._file;
  _hasFile = other._hasFile;
  _hasColor = other._hasColor;
  _enableStdout = other._enableStdout;
  _level = other._level;
  return *this;
}

void Tintin_reporter::log(enum Tintin_reporter::level level,
                          const std::string &message) const {
  switch (level) {
  case DEBUG:
    debug(message);
    break;
  case INFO:
    info(message);
    break;
  case WARNING:
    warning(message);
    break;
  case ERROR:
    error(message);
    break;
  case CRITICAL:
    critical(message);
    break;
  }
}

void Tintin_reporter::debug(const std::string &message) const {
  if (_level > DEBUG) {
    return;
  }
  std::string line = _getPrompt(DEBUG) + " " + message;
  if (_enableStdout && _hasColor) {
    std::cout << DEBUG_COLOR << line << RESET_COLOR << std::endl;
  } else if (_enableStdout) {
    std::cout << line << std::endl;
  }
  if (_hasFile) {
    _appendInLogFile(line);
  }
}

void Tintin_reporter::info(const std::string &message) const {
  if (_level > INFO) {
    return;
  }
  std::string line = _getPrompt(INFO) + " " + message;
  if (_hasColor && _enableStdout) {
    std::cout << INFO_COLOR << line << RESET_COLOR << std::endl;
  } else if (_enableStdout) {
    std::cout << line << std::endl;
  }
  if (_hasFile) {
    _appendInLogFile(line);
  }
}

void Tintin_reporter::warning(const std::string &message) const {
  if (_level > WARNING) {
    return;
  }
  std::string line = _getPrompt(WARNING) + " " + message;
  if (_hasColor && _enableStdout) {
    std::cout << WARNING_COLOR << line << RESET_COLOR << std::endl;
  } else if (_enableStdout) {
    std::cout << line << std::endl;
  }
  if (_hasFile) {
    _appendInLogFile(line);
  }
}

void Tintin_reporter::error(const std::string &message) const {
  if (_level > ERROR) {
    return;
  }
  std::string line = _getPrompt(ERROR) + " " + message;
  if (_hasColor && _enableStdout) {
    std::cout << ERROR_COLOR << line << RESET_COLOR << std::endl;
  } else if (_enableStdout) {
    std::cout << line << std::endl;
  }
  if (_hasFile) {
    _appendInLogFile(line);
  }
}

void Tintin_reporter::critical(const std::string &message) const {
  if (_level > CRITICAL) {
    return;
  }
  std::string line = _getPrompt(CRITICAL) + " " + message;
  if (_hasColor && _enableStdout) {
    std::cout << CRITICAL_COLOR << line << RESET_COLOR << std::endl;
  } else {
    std::cout << line << std::endl;
  }
  if (_hasFile) {
    _appendInLogFile(line);
  }
}

enum Tintin_reporter::level Tintin_reporter::getLevel() const { return _level; }

const std::string &Tintin_reporter::getFile() const { return _file; }

bool Tintin_reporter::hasFile() const { return _hasFile; }

bool Tintin_reporter::hasColor() const { return _hasColor; }

bool Tintin_reporter::hasDatetime() const { return _hasDatetime; }

bool Tintin_reporter::enableStdout() const { return _enableStdout; }

void Tintin_reporter::setFile(const std::string &file) {
  _file = file;
  _hasFile = true;
}

void Tintin_reporter::setEnableStdout(bool enableStdout) {
  _enableStdout = enableStdout;
}

void Tintin_reporter::setLevel(enum Tintin_reporter::level level) {
  _level = level;
}

void Tintin_reporter::setHasColor(bool hasColor) { _hasColor = hasColor; }

void Tintin_reporter::setHasDatetime(bool hasDatetime) {
  _hasDatetime = hasDatetime;
}

std::string Tintin_reporter::_getPrompt(enum level level) const {
  std::string datetime;

  if (_hasDatetime) {
    std::time_t now = std::time(NULL);
    struct std::tm timeinfo;
    char buffer[50];

    localtime_r(&now, &timeinfo);
    const char *format = "[%d/%m/%Y-%H:%M:%S] - ";
    std::strftime(buffer, sizeof(buffer), format, &timeinfo);
    datetime = std::string(buffer);
  }
  switch (level) {
  case DEBUG:
    return datetime + "[DEBUG] -";
  case INFO:
    return datetime + "[INFO] -";
  case WARNING:
    return datetime + "[WARNING] -";
  case ERROR:
    return datetime + "[ERROR] -";
  case CRITICAL:
    return datetime + "[CRITICAL] -";
  }
  return datetime;
}

void Tintin_reporter::_appendInLogFile(const std::string &line) const {
  std::ofstream logFile(_file.c_str(), std::ios::app);

  if (logFile.is_open()) {
    logFile << line << std::endl;
  }
}