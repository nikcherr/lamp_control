#pragma once
#include <iostream>
#include <chrono>

class LogDuration
{
public:
    LogDuration(const std::string& message)
        : _message(message + ": "),
          _start(std::chrono::steady_clock::now()) { }
    ~LogDuration()
    {
        using namespace std::chrono;
        auto _finish = steady_clock::now();
        auto duration = _finish - _start;
        std::cout << _message << duration_cast<milliseconds>(duration).count() << " ms" << std::endl;
    }
private:
    std::string _message;
    std::chrono::steady_clock::time_point _start;
};

#define UNIQ_VARIABLE_NAME_IMPL(line) __variable__##line
#define UNIQ_VARIABLE_NAME(line) UNIQ_VARIABLE_NAME_IMPL(line)

#define LOG_DURATION(message) \
  LogDuration UNIQ_VARIABLE_NAME(__LINE__) {message};
