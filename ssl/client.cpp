#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

enum { max_length = 1024 };

class client
{
    public:
        client(boost::asio::io_service& io_service,
                boost::asio::ssl::context& context,
                boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
            : socket_(io_service, context)
        {
            // 开始验证服务端
            socket_.set_verify_mode(boost::asio::ssl::verify_peer);
            socket_.set_verify_callback(
                    boost::bind(&client::verify_certificate, this, _1, _2));

            // 开始握手, 握手完成则开始读写tcp数据
            boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
                    boost::bind(&client::handle_connect, this,
                        boost::asio::placeholders::error));
        }

        bool verify_certificate(bool preverified,
                boost::asio::ssl::verify_context& ctx)
        {
            // The verify callback can be used to check whether the certificate that is
            // being presented is valid for the peer. For example, RFC 2818 describes
            // the steps involved in doing this for HTTPS. Consult the OpenSSL
            // documentation for more details. Note that the callback is called once
            // for each certificate in the certificate chain, starting from the root
            // certificate authority.

            // In this example we will simply print the certificate's subject name.


            if(!preverified) return false;

            char subject_name[256];
            X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
            X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

            std::cout << "["
                << socket_.lowest_layer().remote_endpoint().address().to_string()
                << ":"
                << socket_.lowest_layer().remote_endpoint().port()
                << "]"
                << " verifying:"
                << subject_name
                << std::endl;

            return true;
        }

        void handle_connect(const boost::system::error_code& error)
        {
            if (!error)
            {
                std::cout << "[" 
                          << socket_.lowest_layer().remote_endpoint().address().to_string()
                          << ":"
                          << socket_.lowest_layer().remote_endpoint().port()
                          << "] connected"
                          << std::endl;

                socket_.async_handshake(boost::asio::ssl::stream_base::client,
                        boost::bind(&client::handle_handshake, this,
                            boost::asio::placeholders::error));
            }
            else
            {
                std::cout << "[server] connect failed"
                          << " error:"
                          << error.message()
                          << std::endl;
            }
        }

        void handle_handshake(const boost::system::error_code& error)
        {
            if (!error)
            {
                // 异步写
                std::cout << "Enter message: ";
                std::cin.getline(request_, max_length);
                size_t request_length = strlen(request_);
                boost::asio::async_write(socket_,
                        boost::asio::buffer(request_, request_length),
                        boost::bind(&client::handle_write, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
            }
            else
            {
                std::cout << "[server] handshake failed"
                          << " error:"
                          << error.message()
                          << std::endl;
            }
        }

        // 写完
        void handle_write(const boost::system::error_code& error,
                          size_t bytes_transferred)
        {
            if (!error)
            {
                // 异步读
                boost::asio::async_read(socket_,
                        boost::asio::buffer(reply_, bytes_transferred),
                        boost::bind(&client::handle_read, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
            }
            else
            {

                std::cout << "[server] write failed"
                          << " error:"
                          << error.message()
                          << std::endl;
            }
        }

        // 读完
        void handle_read(const boost::system::error_code& error,
                size_t bytes_transferred)
        {
            if (!error)
            {
                std::cout << "Reply: ";
                std::cout.write(reply_, bytes_transferred);
                std::cout << "\n";

                // 异步写
                std::cout << "Enter message: ";
                std::cin.getline(request_, max_length);
                size_t request_length = strlen(request_);
                boost::asio::async_write(socket_,
                        boost::asio::buffer(request_, request_length),
                        boost::bind(&client::handle_write, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
            }
            else
            {
                std::cout << "[server] read failed"
                          << " error:"
                          << error.message()
                          << std::endl;
            }
        }

    private:
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket>  socket_;
        char                                                     request_[max_length];
        char                                                     reply_[max_length];
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(argv[1], argv[2]);
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

        boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);

        // CA证书, 用于验证服务端证书
        ctx.load_verify_file("./certificate/ca-cert.pem");
        // 客户端证书, 用于被服务端验证(单向验证可省略)
        ctx.use_certificate_chain_file("certificate/client-cert.pem");
        // 客户端密钥, 用于客户端加密(单向验证可省略)
        ctx.use_private_key_file("certificate/client-cert.key", boost::asio::ssl::context::pem);

        client c(io_service, ctx, iterator);

        io_service.run();

        std::cout << "out run" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

