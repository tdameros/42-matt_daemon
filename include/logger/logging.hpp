#ifndef LOGGER_LOGGING_HPP
#define LOGGER_LOGGING_HPP

#include "logger/Tintin_reporter.hpp"

namespace logging {

static Tintin_reporter logger;

void log(enum Tintin_reporter::level level, const std::string &message);
void debug(const std::string &message);
void info(const std::string &message);
void warning(const std::string &message);
void error(const std::string &message);
void critical(const std::string &message);

enum Tintin_reporter::level getLevel();
const std::string &getFile();
bool hasFile();
bool hasColor();
bool hasDatetime();
bool enableStdout();

void setFile(const std::string &file);
void setLevel(enum Tintin_reporter::level level);
void setHasColor(bool hasColor);
void setHasDatetime(bool hasDatetime);
void setEnableStdout(bool enableStdout);

} // namespace logging

#endif
