#include <iostream>
#include <utility>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include "WeatherFetcher.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

using namespace asio;
using namespace boost::property_tree;

WeatherFetcher::WeatherFetcher(boost::asio::io_service& io_service) : service_(io_service) {}

ptree WeatherFetcher::fetchData(const std::string& location) {
    std::string geocoding_url = "https://geocoding-api.open-meteo.com/v1/search?name="
        + location + "&count=1&language=en&format=json";
    ptree geocoding_pt = getJsonFromUrl(geocoding_url);

    double latitude, longitude;
    for (const auto& result : geocoding_pt.get_child("results")) {
        latitude = result.second.get<double>("latitude");
        longitude = result.second.get<double>("longitude");
        break;
    }

    std::string forecast_url = "https://api.open-meteo.com/v1/forecast?latitude=" + std::to_string(latitude) +
        "&longitude=" + std::to_string(longitude) +
        "&hourly=temperature_2m,rain";

    ptree weather_data = getJsonFromUrl(forecast_url);
    return weather_data;
}

ptree WeatherFetcher::getJsonFromUrl(const std::string& url) {
    io_context io;
    tcp::resolver resolver(io);
    beast::tcp_stream stream(io);

    std::string host = url.substr(url.find("://") + 3, url.find("/", 
        url.find("://") + 3) - (url.find("://") + 3));
    std::string target = url.substr(url.find("/", url.find("://") + 3));

    http::request <http::string_body> req { http::verb::get, target, 11 };
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    auto const results = resolver.resolve(host, "http");
    stream.connect(results);

    http::write(stream, req);
    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(stream, buffer, res);

    std::string json_response = beast::buffers_to_string ( res.body().data() );

    std::istringstream string_stream(json_response);
    ptree properties;
    json_parser::read_json(string_stream, properties);

    return properties;
}
