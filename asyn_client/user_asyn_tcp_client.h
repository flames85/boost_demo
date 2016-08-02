//
// Created by Shao.Admin on 16/7/28.
//

#ifndef USER_ASYN_TCP_CLIENT_H
#define USER_ASYN_TCP_CLIENT_H

#include "asyn_tcp_client.h"
#include "run_timer.h"

class user_asyn_tcp_client : public asyn_tcp_client
{

public:
    // 析构
    virtual ~user_asyn_tcp_client();

    boost::shared_ptr<user_asyn_tcp_client> shared_from_this()
    {
        return boost::dynamic_pointer_cast<user_asyn_tcp_client>(asyn_tcp_client::shared_from_this());
    }

    typedef boost::shared_ptr<user_asyn_tcp_client> ptr;
    static ptr create(shared_io s_io, const ip::tcp::endpoint &ep, const std::string &auth_id, const std::string &auth_pwd);

    void start();

    // 回调函数
    virtual void did_connect_to_host(const ip::tcp::endpoint &ep) ;
    virtual void did_disconnect(const ip::tcp::endpoint &ep) ;
    virtual void did_read_data(const char* data, size_t len) ;
    virtual void did_write_bytes(size_t bytes) ;

    // 定时器回调
    static void* timeout_callback(boost::shared_ptr<user_asyn_tcp_client> s_obj);

protected:

    // 隐藏的构造
    user_asyn_tcp_client(shared_socket s_socket,
                       shared_io s_io,
                       const ip::tcp::endpoint &ep,
                       const std::string &auth_id,
                       const std::string &auth_pwd);

private:

    void do_login();

private:
    // io service
    shared_io                           s_io_;
    // 定时器类
    run_timer<user_asyn_tcp_client>      *timer_;

    // 用户名密码
    std::string                         auth_id_;
    std::string                         auth_pwd_;

    // 报文的seq
    u_int64_t                           seq_;
};


#endif // USER_ASYN_TCP_CLIENT_H
