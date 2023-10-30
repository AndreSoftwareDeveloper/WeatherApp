#include <string>
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class WeatherFetcher {

    public:
        WeatherFetcher(boost::asio::io_service& service);

        boost::property_tree::ptree fetchData(const std::string& location);
        boost::property_tree::ptree getJsonFromUrl(const std::string& url);

    private:
        boost::asio::io_service& service_;        
};
