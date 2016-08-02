//
// Created by Shao.Admin on 16/8/1.
//
#ifndef LOG_FOR_CXX_H
#define LOG_FOR_CXX_H

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

class log_for_cxx
{
public:
    enum log_level
    {
        trace,
        debug,
        info,
        warning,
        error,
        fatal
    };
    static log_for_cxx* create_global_logger(const std::string &config_file, const std::string &logger_name);
    static log_for_cxx* get_instance();

    void writeFormat(log_level level, const char *format, ...);
    void writeFormat(log_level level, const char* func, int line, const char *format, ...);

protected:
    log_for_cxx(const std::string &config_file, const std::string &logger_name);
    ~log_for_cxx();

private:
    log4cxx::LoggerPtr          logger_;
    static log_for_cxx          *instance_;
};

#define log_trace(format, ...)    log_for_cxx::get_instance()->writeFormat(log_for_cxx::trace, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define log_debug(format, ...)    log_for_cxx::get_instance()->writeFormat(log_for_cxx::debug, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define log_info(format, ...)    log_for_cxx::get_instance()->writeFormat(log_for_cxx::info, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define log_warn(format, ...)    log_for_cxx::get_instance()->writeFormat(log_for_cxx::warning, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define log_error(format, ...)    log_for_cxx::get_instance()->writeFormat(log_for_cxx::error, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define log_fatal(format, ...)    log_for_cxx::get_instance()->writeFormat(log_for_cxx::fatal, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#endif //LOG_FOR_CXX_H
