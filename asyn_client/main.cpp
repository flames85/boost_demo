#include "user_asyn_tcp_client.h"
#include "config_parser.h"
#include "log_for_cxx.h"
#include <boost/thread/thread.hpp>

void hello()
{
    std::cout << boost::this_thread::get_id() << std::endl;
    log_debug("Hello world, I'm a thread!");
}

int main(int argc, char* argv[])
{
    // 配置文件
    int n_ret = CONF.parse(argc, argv) ;
    if( 0 != n_ret )
    {
        return n_ret;
    }

    std::string server_ip;
    int server_port;
    std::string client_user;
    std::string client_password;
    std::string log_conf_file;

    bool bConfOk = true;
    std::string errorDesc;
    do {
        if(!CONF.count("server.ip"))
        {
            errorDesc += "server.ip not found";
            bConfOk = false;
            break;
        }
        server_ip = CONF.get<std::string>("server.ip");

        if(!CONF.count("server.port"))
        {
            errorDesc += "server.port not found";
            bConfOk = false;
            break;
        }
        server_port = CONF.get<int>("server.port");

        if(!CONF.count("client.user"))
        {
            errorDesc += "client.user not found";
            bConfOk = false;
            break;
        }
        client_user = CONF.get<std::string>("client.user");

        if(!CONF.count("client.password"))
        {
            errorDesc += "client.password not found";
            bConfOk = false;
            break;
        }
        client_password = CONF.get<std::string>("client.password");


        if(!CONF.count("log.path"))
        {
            errorDesc += "log.path not found";
            bConfOk = false;
            break;
        }
        log_conf_file = CONF.get<std::string>("log.path");

    } while(0);
    if(!bConfOk)
    {
        std::cout << "read conf error: " << errorDesc << std::endl;
        return -1;
    }

    // 日志配置
    log_for_cxx::create_global_logger(log_conf_file, "main");

    // 创建io
    shared_io s_io(new io_service);
    // 连接remote
    ip::tcp::endpoint ep( ip::address::from_string(server_ip), server_port);
    user_asyn_tcp_client::ptr client = user_asyn_tcp_client::create(s_io, ep, client_user, client_password);

    boost::thread thrd(&hello);

    // 运行
    s_io->run();

    log_info("bye!");

    return 0;
}

