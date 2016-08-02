//
// Created by Shao.Admin on 16/7/29.
//

#include "config_parser.h"

boost::shared_ptr<config_parser> config_parser::config_; // 静态成员
void config_parser::help()
{
    std::cout << desccfg_;
}

int config_parser::clear()
{
    cfg_.reset();
    cfg_ = boost::shared_ptr<boost::program_options::variables_map>(new boost::program_options::variables_map);
    return 0;
}

int config_parser::parse(int argc, char* argv[])
{
    try
    {
        clear();
        if(argc != 0 || argv != 0)
        {
            this->argc_ = argc;
            this->argv_ = argv;
        }

        std::string brief("print this help.");
        desccfg_.add_options ()

                // 仅仅显示
                ("help,h", brief.data())

                // 用这种方式可以添加其他参数
//                ("other,o", boost::program_options::value<std::string>(), "other cmd line")

                // version
                ("version,v", "display the version of me and exit")

                // 当前配置文件
                ("config-file,c", boost::program_options::value<std::string>(), "use config file.")

                // 读取配置中信息
                // (布尔型配置)
                //                ("client.daemon,e", boost::program_options::value<bool>()->default_value(true),"run as daemon")
                // 日志(字符串型配置)
                ("log.path,l", boost::program_options::value<std::string>()->default_value("../log/"), "special set log file path.")

                // server ip
                ("server.ip,x", boost::program_options::value<std::string>()->default_value("127.0.0.1"), "special set server ip.")
                // server port
                ("server.port,X", boost::program_options::value<int>()->default_value(0), "special set server port.")
                ("client.user,I", boost::program_options::value<std::string>()->default_value("root"), "special set client user.")
                ("client.password,P", boost::program_options::value<std::string>()->default_value("12345"), "special set client password.");

        boost::program_options::positional_options_description p;
        p.add("config-file", -1);

        store (boost::program_options::command_line_parser(this->argc_, this->argv_).
                       options(desccfg_).
                       positional(p).
                       allow_unregistered().
                       run(),
               *cfg_);

        notify(*cfg_);

        if (!(*cfg_).count("help"))
        {
            if (!(*cfg_).count("config-file"))
            {
                std::cerr << "usage: " << argv[0] << "-c <config-file> \n";
                return -1;
            }

            std::ifstream ifs((*cfg_)["config-file"].as<std::string>().c_str());
            store(parse_config_file(ifs, desccfg_), *cfg_, true);
            ifs.close();
            notify(*cfg_);
        } else {
            help();
            return 1;
        }

    }
    catch(std::exception& e)
    {
        std::cout << e.what() << "\n";
        return -2;
    }

    return 0;
}

config_parser& config_parser::instance()
{
    if (config_parser::config_.get() == 0)
    {
        config_parser::config_.reset(new config_parser);
    }
    return *config_parser::config_;
}