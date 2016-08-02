//
// Created by Shao.Admin on 16/7/28.
//

#include "asyn_tcp_client.h"
#include "log_for_cxx.h"

typedef asyn_tcp_client Self_type;
#define BIND_THIS_1(fun, arg1)          boost::bind(&Self_type::fun, shared_from_this(), arg1)
#define BIND_THIS_2(fun, arg1, arg2)    boost::bind(&Self_type::fun, shared_from_this(), arg1, arg2)

asyn_tcp_client::asyn_tcp_client(shared_socket s_socket,
                                 const ip::tcp::endpoint &ep) :
        s_socket_(s_socket),
        ep_(ep)
{

}

asyn_tcp_client::~asyn_tcp_client() {

}

void asyn_tcp_client::start()
{
    if(s_socket_)
    {
        s_socket_->async_connect(ep_, BIND_THIS_1(on_connect, _1));
    }
}

void asyn_tcp_client::stop()
{
    if(s_socket_->is_open()) {
        s_socket_->close();
    }
}

bool asyn_tcp_client::connected()
{
    return s_socket_->is_open();
}

void asyn_tcp_client::do_read()
{
    if ( !connected() ) return;
    s_socket_->async_read_some(buffer(read_buffer_, max_msg), BIND_THIS_2(on_read, _1, _2));
}

void asyn_tcp_client::do_write(const std::string & msg)
{
    if ( !connected() ) return;
    s_socket_->async_write_some( buffer(msg, msg.size()), BIND_THIS_2(on_write, _1, _2));
}

void asyn_tcp_client::on_connect(const error_code & err)
{
    if ( !err)
    {
        log_debug("connected");
        // 回调
        did_connect_to_host(ep_);
        // 去读
        do_read();
    }
    else {
        log_warn("error:%s", err.message().c_str());
        // 回调
        did_disconnect(ep_);
        stop();
    }
}
void asyn_tcp_client::on_read(const error_code & err, size_t bytes)
{
    if ( !err)
    {
        // 回调
        did_read_data(read_buffer_, bytes);
        // 去读
        do_read();
    } else {
        log_warn("error:%s", err.message().c_str());
        did_disconnect(ep_);
        stop();
    }
}
void asyn_tcp_client::on_write(const error_code & err, size_t bytes)
{
    // 回调
    did_write_bytes(bytes);
}