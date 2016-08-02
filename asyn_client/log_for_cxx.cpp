//
// Created by Shao.Admin on 16/8/1.
//

#include <sstream>
#include <iomanip>
#include <iostream>
#include "log_for_cxx.h"

log_for_cxx* log_for_cxx::instance_ = NULL;

log_for_cxx* log_for_cxx::create_global_logger(const std::string &config_file, const std::string &logger_name)
{
    if (instance_)
    {
        delete instance_;
        instance_ = NULL;
    }
    instance_ = new log_for_cxx(config_file, logger_name);
    return instance_;
}

log_for_cxx* log_for_cxx::get_instance()
{
    if (instance_)
    {
        return instance_;
    }
    return NULL;
}

log_for_cxx::log_for_cxx(const std::string &config_file, const std::string &logger_name)
{
    log4cxx::PropertyConfigurator::configure(config_file);
    logger_ = (log4cxx::Logger::getLogger(logger_name));
}

log_for_cxx::~log_for_cxx()
{
}

void log_for_cxx::writeFormat(log_level level, const char* func, int line, const char *format, ...)
{
    try
    {
        char buf_main_msg[1024];
        va_list args;
        va_start(args, format);

        vsprintf(buf_main_msg, format, args);
        va_end(args);

        std::stringstream stream;
        stream << "[" << func << ":" << line << "] - " << buf_main_msg;
        switch(level)
        {
            case trace:
                logger_->trace(stream.str());
                break;
            case debug:
                logger_->debug(stream.str());
                break;
            case info:
                logger_->info(stream.str());
                break;
            case warning:
                logger_->warn(stream.str());
                break;
            case error:
                logger_->error(stream.str());
                break;
            case fatal:
                logger_->fatal(stream.str());
                break;
            default:
                break;

        }
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}
void log_for_cxx::writeFormat(log_level level, const char *format, ...)
{
    char buffer[1024];

    va_list args;
    va_start(args, format);

    vsprintf(buffer, format, args);
    va_end(args);

    switch(level)
    {
        case trace:
            logger_->trace(buffer);
            break;
        case debug:
            logger_->debug(buffer);
            break;
        case info:
            logger_->info(buffer);
            break;
        case warning:
            logger_->warn(buffer);
            break;
        case error:
            logger_->error(buffer);
            break;
        case fatal:
            logger_->fatal(buffer);
            break;
        default:
            break;

    }
}

