//
// Created by Shao.Admin on 16/7/28.
//

#ifndef BOOST_UTIL_H
#define BOOST_UTIL_H


//// 解决(warning: 'register' storage class specifier is deprecated )告警问题
//#if __cplusplus > 199711L
//#define register      // Deprecated in C++11.
//#endif  // #if __cplusplus > 199711L


#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>

using namespace boost::asio;
using namespace boost::system;


typedef boost::shared_ptr<ip::tcp::socket>  shared_socket;
typedef boost::shared_ptr<io_service>       shared_io;


#endif //BOOST_UTIL_H
