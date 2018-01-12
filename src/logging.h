#ifndef TSYM_LOGGING_H
#define TSYM_LOGGING_H

#include <cstring>
#include "logger.h"
#include "sgfy.h"

#define TSYM_LOGGING_ARGS(...) { "tsym", (std::strrchr(__FILE__, '/') + 1), __LINE__,\
    sgfy::str(__VA_ARGS__) }

#define TSYM_DEBUG(...) tsym::Logger::getInstance()->debug(TSYM_LOGGING_ARGS(__VA_ARGS__))
#define TSYM_INFO(...) tsym::Logger::getInstance()->info(TSYM_LOGGING_ARGS(__VA_ARGS__))
#define TSYM_WARNING(...) tsym::Logger::getInstance()->warning(TSYM_LOGGING_ARGS(__VA_ARGS__))
#define TSYM_ERROR(...) tsym::Logger::getInstance()->error(TSYM_LOGGING_ARGS(__VA_ARGS__))
#define TSYM_CRITICAL(...) tsym::Logger::getInstance()->critical(TSYM_LOGGING_ARGS(__VA_ARGS__))

#endif
