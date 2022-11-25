// System Libraries
#include <iostream>
#include <filesystem>

// Opencv
#include <librealsense2/hpp/rs_processing.hpp>
#include <opencv2/opencv.hpp>

// Realsense 2 SDK
#include <librealsense2/hpp/rs_frame.hpp>
#include <librealsense2/rs.hpp>
#include <thread>

#include "Camera.h"


// Camera Manager Namespace
namespace cm {

    // Camera Manager States

    enum CMStates {
        Idle,       // Waiting for Commands
        Setup,      // Camera Initialization
        LogSetup,   // Logging setting initialization
        Logging     // Logging Images
    };

    // Camera Manager Class
    // This class handles all the devices connected to the host
    // The different instances of the devices are handled by the Camera Class
    class CameraManager {
        private:
            Camera camera;

            rs2::context ctx;
            rs2::pipeline pipe;
            rs2::frame_queue queue;

            std::thread frameManagerThread;

            static rs2::device getCamera();

            void saveImage(std::filesystem::path, rs2::frameset, int*);

        public:
            CMStates state = Idle;
            std::mutex countMutex;
            std::map<int, int> counters;

            // Initialize a CameraManager with default parameters
            explicit CameraManager();

            // Pipeline Logging: Save selected frames in a directory
            // Logging preprocessed frames from camera
            // Possiblity to log RBG, Depth, IMU data
            int startLogging(std::filesystem::path, rs2::frame);

            void stopLogging();

            // Log with blocking code
            int startBlockingLogging(std::filesystem::path);

            // Sensor Logging: Low level sensor data logging
            void startSensorLogging(std::filesystem::path);

    };
}

