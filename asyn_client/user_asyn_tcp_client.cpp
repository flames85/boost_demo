//
// Created by Shao.Admin on 16/7/28.
//

#include "user_asyn_tcp_client.h"
#include <boost/format.hpp>
#include "openssl_tool.h"
#include "log_for_cxx.h"

typedef user_asyn_tcp_client Self_type;
#define BIND_THIS_1(fun, arg1)          boost::bind(&Self_type::fun, shared_from_this(), arg1)
#define BIND_THIS_2(fun, arg1, arg2)    boost::bind(&Self_type::fun, shared_from_this(), arg1, arg2)


user_asyn_tcp_client::user_asyn_tcp_client(shared_socket s_socket, shared_io s_io,
                                       const ip::tcp::endpoint &ep,
                                       const std::string &auth_id,
                                       const std::string &auth_pwd) :
        asyn_tcp_client(s_socket, ep),
        s_io_(s_io),
        timer_(new run_timer<user_asyn_tcp_client>(s_io)),
        auth_id_(auth_id),
        auth_pwd_(auth_pwd),
        seq_(0)
{

}

user_asyn_tcp_client::~user_asyn_tcp_client() {

}


user_asyn_tcp_client::ptr user_asyn_tcp_client::create(shared_io s_io,
                                                   const ip::tcp::endpoint &ep,
                                                   const std::string &auth_id,
                                                   const std::string &auth_pwd)
{
    // 创建socket对象
    shared_socket s_socket(new ip::tcp::socket(*s_io));

    // 创建本类对象
    user_asyn_tcp_client::ptr s_obj(new user_asyn_tcp_client(s_socket, s_io, ep, auth_id, auth_pwd));

    // 启动
    s_obj->start();

    // 返回本类对象
    return s_obj;
}
void* user_asyn_tcp_client::timeout_callback(boost::shared_ptr<user_asyn_tcp_client> s_obj) {
    log_debug("call back");
    s_obj->do_login();
    return NULL;
}

void user_asyn_tcp_client::start()
{
    asyn_tcp_client::start();
    timer_->timer_schedule(5, user_asyn_tcp_client::timeout_callback, shared_from_this());
}

void user_asyn_tcp_client::do_login() {

    // send
    do_write("login");
}

void user_asyn_tcp_client::did_connect_to_host(const ip::tcp::endpoint &ep) {
    log_debug("end-point: %s:%d", ep.address().to_string().c_str(), ep.port());
    // 登录
//    do_login();
}

void user_asyn_tcp_client::did_disconnect(const ip::tcp::endpoint &ep) {
    log_warn("end-point: %s:%d", ep.address().to_string().c_str(), ep.port());
}

void user_asyn_tcp_client::did_read_data(const char* data, size_t len) {
    std::string copy(data, len);
    log_debug("recv: %s", copy.c_str());
}

void user_asyn_tcp_client::did_write_bytes(size_t bytes) {
    log_debug("size: %lu", bytes);
}