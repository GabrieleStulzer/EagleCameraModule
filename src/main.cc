#include <librealsense2/rs.hpp>

#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <chrono>

#include "../include/CameraManager.h"

using namespace cm;

int main() {

    CameraManager CM;

    CM.startLogging("test/", nullptr);

    while(true) {
        std::cout << "Logging Images" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    return 0;
}

