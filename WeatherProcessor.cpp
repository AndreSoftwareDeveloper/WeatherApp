#include <iostream>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/format.hpp>
#include "WeatherProcessor.h"

using namespace std;
WeatherProcessor::WeatherProcessor() {}

void WeatherProcessor::startProcessing(WeatherFetcher& fetcher, const string& location) {
    boost::property_tree::ptree weather_data = fetcher.fetchData(location);
    boost::thread processingThread(&WeatherProcessor::processWeatherData, this, weather_data, location);
    processingThread.join();
}

void WeatherProcessor::processWeatherData(const boost::property_tree::ptree& properties, const string& location) {
    double temperature, rainfall;
    string time;
    vector<double> temperatures, rainfalls;
    vector<string> times;

    for (const auto& entry : properties.get_child("hourly"))
        if (entry.first == "time")
            for (const auto& timeEntry : entry.second) {
                time = timeEntry.second.get_value<string>();
                times.push_back(time);
            }
        else if (entry.first == "temperature_2m")
            for (const auto& temperatureEntry : entry.second) {
                temperature = temperatureEntry.second.get_value<double>();
                temperatures.push_back(temperature);
            }
        else if (entry.first == "rain")
            for (const auto& rainfallEntry : entry.second) {
                rainfall = rainfallEntry.second.get_value<double>();
                rainfalls.push_back(rainfall);                    
            }                   

    cout << "---------------------------------REPORT------------------------------------" << endl;
    for (size_t i = 0; i < temperatures.size(); ++i)
        if (temperatures[i] < 10.0 && rainfalls[i] > 0.0) {
            stringstream alert;
                alert << boost::format("Warning %s, low temperature %.6f of C and rain %.6fmm expected on %.6f")
                    % location % temperatures[i] % rainfalls[i] % times[i];            
            cout << alert.str() << endl;
        }
}
