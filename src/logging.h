#ifndef TSYM_LOGGING_H
#define TSYM_LOGGING_H

#include <boost/format.hpp>
#include <cstring>
#include <iterator>
#include "logger.h"

namespace tsym {
    namespace detail {
        template <class S, class... T> std::string logFormat(S&& fmt, const T&... args)
        {
            boost::format format(std::forward<S>(fmt));

            return boost::str((format % ... % args));
        }
    }
}

#define TSYM_LOGGING_ARGS(...)                                                                                         \
    {                                                                                                                  \
        "tsym", std::next(std::strrchr(__FILE__, '/')), __LINE__, tsym::detail::logFormat(__VA_ARGS__)                 \
    }

#define TSYM_DEBUG(...) tsym::Logger::getInstance().debug(TSYM_LOGGING_ARGS(__VA_ARGS__))
#define TSYM_INFO(...) tsym::Logger::getInstance().info(TSYM_LOGGING_ARGS(__VA_ARGS__))
#define TSYM_WARNING(...) tsym::Logger::getInstance().warning(TSYM_LOGGING_ARGS(__VA_ARGS__))
#define TSYM_ERROR(...) tsym::Logger::getInstance().error(TSYM_LOGGING_ARGS(__VA_ARGS__))
#define TSYM_CRITICAL(...) tsym::Logger::getInstance().critical(TSYM_LOGGING_ARGS(__VA_ARGS__))

#endif
