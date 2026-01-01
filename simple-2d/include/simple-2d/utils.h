#ifndef SIMPLE_2D_UTILS_H
#define SIMPLE_2D_UTILS_H
#include <plog/Log.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Init.h>

#define SIMPLE_2D_LOG_ERROR PLOGE_(0)
#define SIMPLE_2D_LOG_WARNING PLOGW_(0)
#define SIMPLE_2D_LOG_INFO PLOGI_(0)
#define SIMPLE_2D_LOG_DEBUG PLOGD_(0)


#define SIMPLE_2D_LOG_ERROR_(instance) PLOGE_(instance)
#define SIMPLE_2D_LOG_WARNING_(instance) PLOGW_(instance)
#define SIMPLE_2D_LOG_INFO_(instance) PLOGI_(instance)
#define SIMPLE_2D_LOG_DEBUG_(instance) PLOGD_(instance)


namespace simple_2d {
    enum LogLevel {
        ERROR,
        WARNING,
        INFO,
        DEBUG,
    };
    
    template<int instanceId>
    void SetupLog(LogLevel logLevel) {
        static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::Severity mappedLogLevel = plog::Severity::none;
        switch (logLevel) {
        case LogLevel::ERROR:
            mappedLogLevel = plog::Severity::error;
        case LogLevel::WARNING:
            mappedLogLevel = plog::Severity::warning;
        case LogLevel::INFO:
            mappedLogLevel = plog::Severity::info;
        case LogLevel::DEBUG:
            mappedLogLevel = plog::Severity::debug;
        }
        plog::init<instanceId>(mappedLogLevel, &consoleAppender);
    }
};

#endif
