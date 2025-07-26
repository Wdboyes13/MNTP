#include <utility>
#include <boost/asio.hpp>
#include <exception>
#include <iostream>

using boost::asio::ip::tcp;

#define SEND(Data) boost::asio::write(socket, boost::asio::buffer( Data ))

#define MKB(Num) auto buf##Num = RecvBytes(&socket)
#define BTS(Num) std::string str##Num (buf##Num.data())

std::array<char, 1024> RecvBytes(boost::asio::ip::tcp::socket* socket){
    std::array<char, 1024> buffer; // You can change the size as needed

    boost::system::error_code error;
    std::size_t bytes_read = socket->read_some(boost::asio::buffer(buffer), error);

    if (error == boost::asio::error::eof) {
        // Connection closed cleanly by peer.
        std::cout << "Client disconnected." << std::endl;
    } else if (error) {
                throw boost::system::system_error(error); // Some other error.
    }
    return buffer;
}

int main(){
    std::cout << "Please Enter a TZ Time Zone: ";
    std::string timez;
    std::cin >> timez;
    std::cout << "Please enter the IP of the MNTP Server: ";
    std::string ServerIP;
    std::cin >> ServerIP;
    std::cout << "Is the server on the default port [Y/N]: ";
    std::string IsOnPort;
    std::cin >> IsOnPort;
    std::string port = "6980";
    if (IsOnPort == "N" || IsOnPort == "n"){
        std::cout << "Please Enter the Port to use: ";
        std::cin >> port;
    }

    try {
        boost::asio::io_context io_ctx;
        tcp::resolver resolver(io_ctx);
        tcp::resolver::results_type endpoints = resolver.resolve(ServerIP, port);
        tcp::socket socket(io_ctx);
        boost::asio::connect(socket, endpoints);
        SEND("CNT\r\n");
        MKB(1);
        BTS(1);
        if (str1 == "CNTD\r\n"){
            SEND(timez + "\r\n");
            MKB(2);
            BTS(2);
            if (str2 == "RECV-20\r\n"){
                SEND("R1\r\n");
                MKB(3);
                BTS(3);
                str3.erase(std::remove_if(str3.begin(), str3.end(),
                    [](char c) { return c == '\r' || c == '\n'; }),
                        str3.end());
                std::cout << str3 << std::endl;
            } else {
                throw std::runtime_error("Server Error");
            }
        } else {
            throw std::runtime_error("Didn't Connect");
        }
        
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}