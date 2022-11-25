#include "../include/Camera.h"
#include <iostream>

using namespace cm;

Camera::Camera(rs2::device camera) {
    this->camera = camera;
}

void Camera::printSensorsNames() {
    std::vector<rs2::sensor> sensors = this->camera.query_sensors();

    int index = 0;
    // We can now iterate the sensors and print their names
    for (rs2::sensor sensor : sensors)
        if (sensor.supports(RS2_CAMERA_INFO_NAME)) {
            ++index;
            if (index == 1) {
                sensor.set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE, 1);
                // sensor.set_option(RS2_OPTION_AUTO_EXPOSURE_LIMIT,50000);
                sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 1); // emitter on for depth information
            }
            std::cout << "  " << index << " : " << sensor.get_info(RS2_CAMERA_INFO_NAME) << std::endl;
            // get_sensor_option(sensor);
            if (index == 2){
                // RGB camera
                sensor.set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE,1);

                // sensor.set_option(RS2_OPTION_EXPOSURE,80.f);
            }

            if (index == 3){
                sensor.set_option(RS2_OPTION_ENABLE_MOTION_CORRECTION,0);
            }

        }
}
