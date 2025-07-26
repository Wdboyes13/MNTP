#include <utility>
#include <boost/asio.hpp>
#include <iostream>
#include <chrono>
#include "../date/include/date/tz.h"

namespace MNTP {
    struct MTime {
        int MSecs;
        int MHours;
        int MMins;
    };
    MTime convertToMetricTime(int hour, int minute, int second) {
        int mHours = hour / 2.4;
        int mMins = minute / 2.4;
        int mSecs = second / 1.44;

        return { mSecs, mHours, mMins };
    }
    std::string ConstructMetricTimePacket(const std::string* TZDTimeZone){
        try{
        auto now = std::chrono::system_clock::now();
        auto tz = date::locate_zone(*TZDTimeZone);
        date::zoned_time zt{tz, now};
        auto local = std::chrono::floor<std::chrono::seconds>(zt.get_local_time());
        std::time_t tt = std::chrono::system_clock::to_time_t(zt.get_sys_time());

        std::tm* tm = std::localtime(&tt);
        MTime time = MNTP::convertToMetricTime(tm->tm_hour, tm->tm_min, tm->tm_sec);
        std::string TimePacket;
        TimePacket.append(std::to_string(time.MHours));
        TimePacket.append("-");
        TimePacket.append(std::to_string(time.MMins));
        TimePacket.append("-");
        TimePacket.append(std::to_string(time.MSecs));
        TimePacket.append("\r\n");
        return TimePacket;
        } catch (const std::runtime_error& e) {
        std::cerr << "Invalid timezone: " << e.what() << std::endl;
        return "ERR";
        }
    }
}

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
    try {
        boost::asio::io_context io_ctx;
        boost::asio::ip::tcp::acceptor acceptor(io_ctx, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6980));
        std::cout << "MNTP Server Started on Port 6980" << std::endl;
        while (true){
            boost::asio::ip::tcp::socket socket(io_ctx);
            acceptor.accept(socket);
            // --- Receive bytes from the client ---
            auto buf1 = RecvBytes(&socket);
            std::string str1(buf1.data()); 
            if (str1 == "CNT\r\n"){
                std::string message = "CNTD\r\n";
                boost::asio::write(socket, boost::asio::buffer(message));
                auto buf2 = RecvBytes(&socket);
                std::string str = buf2.data();
                size_t pos = str.find("\r\n");
                if (pos != std::string::npos) {
                    str.erase(pos);  // remove from CRLF to end
                }
                std::string TimePack = MNTP::ConstructMetricTimePacket(&str);
                if (TimePack == "ERR"){
                    std::cerr << "Failed to get Time Zone Packet" << std::endl;
                    boost::asio::write(socket, boost::asio::buffer("DD\r\n"));
                    socket.close();
                } else {
                    boost::asio::write(socket, boost::asio::buffer("RECV-20\r\n"));
                    auto buf3 = RecvBytes(&socket);
                    std::string str2(buf3.data());
                    if (str2 == "R1\r\n"){
                        std::cout << "Fully sent off time" << std::endl;
                        boost::asio::write(socket, boost::asio::buffer(TimePack + "\r\n"));
                        socket.close();
                    } else {
                        std::cerr << "Unexpected Data Expected: R1\\r\\n | Got: " + str2 << std::endl;
                        boost::asio::write(socket, boost::asio::buffer("DD\r\n"));
                        socket.close();
                    }
                }
            } else {
                std::cerr << "Failed To Connect" << std::endl;
                boost::asio::write(socket, boost::asio::buffer("DD\r\n"));
                socket.close();
            }
        }
    } catch (std::exception &e){
        std::cerr << "Server Exception: " << e.what() << std::endl;
    }
}