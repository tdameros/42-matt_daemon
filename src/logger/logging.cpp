#include "logger/logging.hpp"

namespace logging {

void log(enum Tintin_reporter::level level, const std::string &message) {
  logger.log(level, message);
}

void debug(const std::string &message) { logger.debug(message); }

void info(const std::string &message) { logger.info(message); }

void warning(const std::string &message) { logger.warning(message); }

void error(const std::string &message) { logger.error(message); }

void critical(const std::string &message) { logger.critical(message); }

enum Tintin_reporter::level getLevel() { return logger.getLevel(); }

const std::string &getFile() { return logger.getFile(); }

bool hasFile() { return logger.hasFile(); }

bool hasColor() { return logger.hasColor(); }

bool hasDatetime() { return logger.hasDatetime(); }

bool enableStdout() { return logger.enableStdout(); }

void setFile(const std::string &file) { logger.setFile(file); }

void setLevel(enum Tintin_reporter::level level) { logger.setLevel(level); }

void setHasColor(bool hasColor) { logger.setHasColor(hasColor); }

void setHasDatetime(bool hasDatetime) { logger.setHasDatetime(hasDatetime); }

void setEnableStdout(bool enableStdout) {
  logger.setEnableStdout(enableStdout);
}

} // namespace logging
