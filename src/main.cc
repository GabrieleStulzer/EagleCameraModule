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
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Logging Images" << std::endl;
    }

    return 0;
}

