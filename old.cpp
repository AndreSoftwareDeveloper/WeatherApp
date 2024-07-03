#include <iostream>
#include <string>
#include <vector>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;

class WeatherFetcher {
public:
    WeatherFetcher(boost::asio::io_service& io_service) : io_service_(io_service) {}

    void fetchData(const string& location) {
        // Simulate fetching data from the open-meteo API
        // For this example, I'm using a dummy JSON response
        string jsonResponse = "{\"hourly\":{\"time\":[1,2,3,4,5],\"temperature_2m\":[5.9,5.4,5.0,5.0,5.1],\"rain\":[0.1,0.1,0.1,0.4,0.4]}}";
        processWeatherData(jsonResponse);
    }

    void processWeatherData(const string& data) {
        boost::property_tree::ptree pt;
        istringstream is(data);
        boost::property_tree::json_parser::read_json(is, pt);

        vector<double> temperatures, rainfalls;
        for (const auto& entry : pt.get_child("hourly.temperature_2m")) {
            temperatures.push_back(entry.second.get_value<double>());
        }

        for (const auto& entry : pt.get_child("hourly.rain")) {
            rainfalls.push_back(entry.second.get_value<double>());
        }

        for (size_t i = 0; i < temperatures.size(); ++i) {
            if (temperatures[i] < 10.0 || rainfalls[i] > 0.0) {
                stringstream alert;
                alert << "Warning: Low temperature " << temperatures[i] << " of C and rain " << rainfalls[i] << " mm expected on " << i + 1;
                cout << alert.str() << endl;
            }
        }
    }

private:
    boost::asio::io_service& io_service_;
};

class WeatherProcessor {
public:
    WeatherProcessor() {}

    void startProcessing(WeatherFetcher& fetcher) {
        fetcher.fetchData("Wroclaw");
    }
};

int main() {
    boost::asio::io_service io_service;
    WeatherFetcher fetcher(io_service);
    WeatherProcessor processor;

    // Start a separate thread for WeatherProcessor
    boost::thread processorThread(boost::bind(&WeatherProcessor::startProcessing, &processor, boost::ref(fetcher)));

    // Main thread runs the WeatherFetcher
    fetcher.fetchData("Wroclaw");

    // Wait for the processor thread to finish
    processorThread.join();

    return 0;
}
