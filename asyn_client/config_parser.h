//
// Created by Shao.Admin on 16/7/29.
//
// 命令参数解析以及配置文件解析

#include <string>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>


#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

/// 配置解释器 配置文件解析器
class config_parser
{
public:
    int parse(int argc = 0, char* argv[] = 0);

    static config_parser& instance();

    int count(const char* key) { return (*cfg_).count(key); }

    int count(const std::string &key) { return (*cfg_).count(key.c_str()); }

    void help();

    template<typename RET>
    RET get(const char* key) { return (*cfg_)[key].as<RET>(); }
    template<typename RET>
    RET get(const std::string &key) { return (*cfg_)[key.c_str()].as<RET>(); }

private:
    int clear();

public:
    static boost::shared_ptr<config_parser>                     config_;
private:
    boost::shared_ptr<boost::program_options::variables_map>    cfg_;
    boost::program_options::options_description                 desccfg_;
    int                                                         argc_;
    char**                                                      argv_;


};

#define CONF config_parser::instance()

#endif //SI_CLIENT_CONFIG_PARSER_H
