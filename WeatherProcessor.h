#ifndef WEATHERPROCESSOR_H
#define WEATHERPROCESSOR_H

#include "WeatherFetcher.h"

class WeatherProcessor {
    public:
        WeatherProcessor();
        void startProcessing(WeatherFetcher& fetcher, const std::string& location);
        void processWeatherData(const boost::property_tree::ptree& properties, const std::string& location);
};

#endif
