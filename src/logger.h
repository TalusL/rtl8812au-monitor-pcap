#ifndef LOGGER_H
#define LOGGER_H

#include <memory>

#include <string>
#include <iostream>
#include <vector>

#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <functional>

#define uint u_int
#define ushort u_short


class Logger{
public:
  using LCB = std::function<void(const std::string &, const std::string &)>;
  explicit Logger(const LCB& lcb = nullptr){
    m_lcb = lcb;
  }
  template<typename ...ArgsType>
  void error(const std::string_view format,ArgsType &&...args){
    auto fmt_args{std::make_format_args(args...)};
    std::string outstr{vformat(format,fmt_args)};
    if(m_lcb){
      m_lcb("error",outstr);
    }
    fputs(outstr.c_str(),stderr);
    fputs("\n",stderr);
  }
  template<typename ...ArgsType>
  void info(const char* format,ArgsType &&...args){
    auto fmt_args{std::make_format_args(args...)};
    std::string outstr{vformat(format,fmt_args)};
    if(m_lcb){
      m_lcb("info",outstr);
    }
    fputs(outstr.c_str(),stdout);
    fputs("\n",stdout);
  }
  template<typename ...ArgsType>
  void warn(const char* format,ArgsType &&...args){
    auto fmt_args{std::make_format_args(args...)};
    std::string outstr{vformat(format,fmt_args)};
    if(m_lcb){
      m_lcb("v",outstr);
    }
    fputs(outstr.c_str(),stdout);
    fputs("\n",stdout);
  }
  template<typename ...ArgsType>
  void debug(const char* format,ArgsType &&...args){
    auto fmt_args{std::make_format_args(args...)};
    std::string outstr{vformat(format,fmt_args)};
    if(m_lcb){
      m_lcb("debug",outstr);
    }
    fputs(outstr.c_str(),stdout);
    fputs("\n",stdout);
  }
protected:
  LCB m_lcb{};
};

using Logger_t = std::shared_ptr<Logger>;

#endif /* LOGGER_H */
