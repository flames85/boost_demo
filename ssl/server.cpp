#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

class session
{
    public:
        session(boost::asio::io_service& io_service,
                boost::asio::ssl::context& context)
            : socket_(io_service, context)
        {
        }

        ssl_socket::lowest_layer_type& socket()
        {
            return socket_.lowest_layer();
        }

        void start()
        {
            // 开始验证客户端(单向验证可省略)
            socket_.set_verify_mode(boost::asio::ssl::verify_peer);
            socket_.set_verify_callback(
                    boost::bind(&session::verify_certificate, this, _1, _2));

            // 开始握手, 握手完成则开始读写tcp数据
            socket_.async_handshake(boost::asio::ssl::stream_base::server,
                    boost::bind(&session::handle_handshake, this,
                        boost::asio::placeholders::error));
        }

        void handle_handshake(const boost::system::error_code& error)
        {
            if (!error)
            {
                std::cout << "[" 
						  << socket_.lowest_layer().remote_endpoint().address().to_string() 
						  << ":"
						  << socket_.lowest_layer().remote_endpoint().port()
						  << "] connected"
						  << std::endl;
                // 异步读
                socket_.async_read_some(boost::asio::buffer(data_, max_length),
                        boost::bind(&session::handle_read, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
            }
            else
            {
                std::cout << "[" 
						  << socket_.lowest_layer().remote_endpoint().address().to_string() 
						  << ":"
						  << socket_.lowest_layer().remote_endpoint().port()
						  << "] disconnect"
						  << " error:" 
						  << error.message() 
						  << std::endl;
                delete this;
            }
        }

        // (单向验证可省略)
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

            return preverified;
        }

        // 读完
        void handle_read(const boost::system::error_code& error,
                size_t bytes_transferred)
        {
            if (!error)
            {
				std::cout << "[" 
					<< socket_.lowest_layer().remote_endpoint().address().to_string() 
					<< ":"
					<< socket_.lowest_layer().remote_endpoint().port()
					<< "]"
					<< " read:" 
					<< std::string(data_, bytes_transferred) 
					<< std::endl;

                // 异步写
                boost::asio::async_write(socket_,
                        boost::asio::buffer(data_, bytes_transferred),
                        boost::bind(&session::handle_write, this,
                            boost::asio::placeholders::error));
            }
            else
            {
                std::cout << "[" 
						  << socket_.lowest_layer().remote_endpoint().address().to_string() 
						  << ":"
						  << socket_.lowest_layer().remote_endpoint().port()
						  << "] disconnect"
						  << " error:" 
						  << error.message() 
						  << std::endl;
                delete this;
            }
        }

        // 写完
        void handle_write(const boost::system::error_code& error)
        {
            if (!error)
            {
                // 异步读
                socket_.async_read_some(boost::asio::buffer(data_, max_length),
                        boost::bind(&session::handle_read, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
            }
            else
            {
                std::cout << "[" 
						  << socket_.lowest_layer().remote_endpoint().address().to_string() 
						  << ":"
						  << socket_.lowest_layer().remote_endpoint().port()
						  << "] disconnect"
						  << " error:" 
						  << error.message() 
						  << std::endl;
                delete this;
            }
        }

    private:
        enum { 
			max_length = 1024 
		};

        ssl_socket 		socket_;
        char 			data_[max_length];
};

class server
{
    public:
        server(boost::asio::io_service& io_service, unsigned short port)
            : io_service_(io_service),
            acceptor_(io_service,
                    boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
            context_(boost::asio::ssl::context::sslv23)
    {
        context_.set_options(
                boost::asio::ssl::context::default_workarounds
                | boost::asio::ssl::context::no_sslv2
                | boost::asio::ssl::context::single_dh_use);

        context_.set_password_callback(boost::bind(&server::get_password, this));

        // CA证书, 用于验证客户端证书(单向验证可省略)
        context_.load_verify_file("./certificate/ca-cert.pem");
        // 服务端证书, 用于被客户端验证
        context_.use_certificate_chain_file("certificate/server-cert.pem");
        // 服务端密钥, 用于服务端加密
        context_.use_private_key_file("certificate/server-cert.key", boost::asio::ssl::context::pem);

        start_accept();
    }

        std::string get_password() const
        {
            return "test";
        }

        void start_accept()
        {
            session* new_session = new session(io_service_, context_);
            acceptor_.async_accept(new_session->socket(),
                    boost::bind(&server::handle_accept, this, new_session,
                        boost::asio::placeholders::error));
        }

        void handle_accept(session* new_session,
                const boost::system::error_code& error)
        {
            if (!error)
            {
                new_session->start();
            }
            else
            {
                delete new_session;
            }

            start_accept();
        }

    private:
        boost::asio::io_service& io_service_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::asio::ssl::context context_;
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        using namespace std; // For atoi.
        server s(io_service, atoi(argv[1]));

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

