#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "WeatherProcessor.h"

int main() {
    boost::asio::io_service io_service;
    WeatherFetcher fetcher(io_service);
    WeatherProcessor processor;
    std::string location;

    std::cout << "Enter location: ";
    std::cin >> location;

    boost::thread processorThread(boost::bind(&WeatherProcessor::startProcessing, &processor, boost::ref(fetcher), location));

    processorThread.join();

    return 0;
}
