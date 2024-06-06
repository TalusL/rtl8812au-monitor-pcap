#ifndef LOGGER_H
#define LOGGER_H

#include <memory>

#include <fmt/format.h>
#include <string>
#include <iostream>
#include <vector>

#include <format>
#include <iostream>
#include <sstream>
#include <string>

#define uint u_int
#define ushort u_short


class Logger{
public:
  template<typename ...ArgsType>
  void error(const std::string_view format,ArgsType &&...args){
    auto fmt_args{std::make_format_args(args...)};
    std::string outstr{vformat(format,fmt_args)};
    fputs(outstr.c_str(),stderr);
    fputs("\n",stderr);
  }
  template<typename ...ArgsType>
  void info(const char* format,ArgsType &&...args){
    auto fmt_args{std::make_format_args(args...)};
    std::string outstr{vformat(format,fmt_args)};
    fputs(outstr.c_str(),stdout);
    fputs("\n",stdout);
  }
  template<typename ...ArgsType>
  void warn(const char* format,ArgsType &&...args){
    auto fmt_args{std::make_format_args(args...)};
    std::string outstr{vformat(format,fmt_args)};
    fputs(outstr.c_str(),stdout);
    fputs("\n",stdout);
  }
  template<typename ...ArgsType>
  void debug(const char* format,ArgsType &&...args){
    auto fmt_args{std::make_format_args(args...)};
    std::string outstr{vformat(format,fmt_args)};
    fputs(outstr.c_str(),stdout);
    fputs("\n",stdout);
  }
};

using Logger_t = std::shared_ptr<Logger>;

#endif /* LOGGER_H */
