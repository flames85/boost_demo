//
// Created by Shao.Admin on 16/7/28.
//

#ifndef ASYN_TCP_CLIENT_H
#define ASYN_TCP_CLIENT_H


#include <iostream>
#include <string>

#include <boost/thread.hpp>
#include "boost_util.h"

class asyn_tcp_client : public boost::enable_shared_from_this<asyn_tcp_client> , public boost::noncopyable
{
public:
    // 连接函数
    void start();

    // 停止
    void stop();

    // 判断是否已连接
    bool connected();

    // 写入
    void do_write(const std::string & msg);

    // 子类必须实现的回调函数
    virtual void did_connect_to_host(const ip::tcp::endpoint &ep) = 0;
    virtual void did_disconnect(const ip::tcp::endpoint &ep) = 0;
    virtual void did_read_data(const char* data, size_t len) = 0;
    virtual void did_write_bytes(size_t bytes) = 0;

protected:
    // 隐藏的构造函数
    asyn_tcp_client(shared_socket s_socket, const ip::tcp::endpoint &ep) ;

    // 隐藏的析构函数
    virtual ~asyn_tcp_client();

    // 读取
    void do_read();

private:

    // 连接完毕后回调
    void on_connect(const error_code & err);
    // 读取完毕后回调
    void on_read(const error_code & err, size_t bytes);
    // 写入完毕后回调
    void on_write(const error_code & err, size_t bytes);


private:
    shared_socket               s_socket_;
    ip::tcp::endpoint           ep_;


    enum { max_msg = 1024*10 };
    char                        read_buffer_[max_msg];
};


#endif //ASYN_TCP_CLIENT_H
