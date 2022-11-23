#include <librealsense2/rs.hpp>

#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>

#include "../include/CameraManager.h"

using namespace cm;

int main() {

    CameraManager CM;

    CM.startLogging("test/", nullptr);

    while(true) {
        std::cout << "Logging Images" << std::endl;
        usleep(10000000);
    }

    return 0;
}

