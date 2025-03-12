#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>
#include <vector>

template <typename T> std::string toString(const T &value) {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}

void ftSleep(unsigned int milliseconds);

std::vector<std::string> splitDelimiter(const std::string &s, char delimiter);
std::vector<std::string> splitDelimiter(const std::string &s,
                                        const std::string &delimiter);
std::vector<std::string> splitWhiteSpace(const std::string &s);

std::string &trim(std::string &s);
std::string &ltrim(std::string &s);
std::string &rtrim(std::string &s);
std::string &trimCharset(std::string &s, const std::string &charset);
std::string &ltrimCharset(std::string &s, const std::string &charset);
std::string &rtrimCharset(std::string &s, const std::string &charset);

#endif