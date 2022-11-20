// System Libraries
#include <iostream>
#include <filesystem>

// Opencv
#include <opencv2/opencv.hpp>

// Realsense 2 SDK
#include <librealsense2/hpp/rs_frame.hpp>
#include <librealsense2/rs.hpp>


// Camera Manager Namespace
namespace cm {

    // Camera Manager Class
    // This class handles all the devices connected to the host
    // The different instances of the devices are handled by the Camera Class
    class CameraManager {
        private:
            rs2::context ctx;
            rs2::pipeline pipe;
            std::mutex mutex;

        public:
        // Initialize a CameraManager with default parameters
        CameraManager();

        // Pipeline Logging: Save selected frames in a directory
        // Logging preprocessed frames from camera
        // Possiblity to log RBG, Depth, IMU data
        int startLogging(std::filesystem::path directory, rs2::frame* frames);

        // Sensor Logging: Low level sensor data logging
        void startSensorLogging(std::filesystem::path* directory);

    };
}

