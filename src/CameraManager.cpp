#include "../include/CameraManager.h"
#include <cstdlib>
#include <librealsense2/h/rs_frame.h>
#include <librealsense2/h/rs_sensor.h>
#include <mutex>
#include <string.h>
#include <string>
#include <thread>
#include <unistd.h>

using namespace cm;

// Static Functions

rs2::device CameraManager::getCamera() {
    rs2::context ctx;
    return ctx.query_devices()[0];
}


// Constructor Implementation

CameraManager::CameraManager() : camera(getCamera()) {
    this->state = Setup;

    this->ctx = rs2::context();
    this->pipe = rs2::pipeline();

    rs2::device_list devices = ctx.query_devices();

    std::cout << "Librealsense v: " << RS2_API_VERSION_STR << std::endl;
    std::cout << "You have " << ctx.query_devices().size() << " RealSense devices connected" << std::endl;


    if(devices.size() == 0) {
        std::cerr << "No device connected" << std::endl;
    }

    std::cout << "Sensors Available on Camera: " << std::endl;
    this->camera.printSensorsNames();

    this->state = Idle;
}


// PUBLIC Functions Implementation

int CameraManager::startLogging(std::filesystem::path directory, rs2::frame frames) try
{
    this->state = LogSetup;

    int counter = 0;
    std::stringstream filepath;
    std::mutex mutex;


    rs2::frame_queue framesQueue;
    rs2::config cfg;

    filepath << directory.string();
    filepath << "StreamBackup.bag";
    cfg.enable_record_to_file(filepath.str());

    cfg.enable_stream(RS2_STREAM_COLOR, 1280, 720, RS2_FORMAT_BGR8,30);
    cfg.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);

    cfg.enable_stream(RS2_STREAM_INFRARED, 1, 1280, 720, RS2_FORMAT_Y8, 30);
    cfg.enable_stream(RS2_STREAM_INFRARED, 2, 1280, 720, RS2_FORMAT_Y8, 30);

    std::cout << "Start Pipeline" << std::endl;
    this->pipe.start(cfg);
    this->state = Logging;

    // TODO: Save IMU Data
    // PERF: Check for performance Issues during logging

    auto frameManager = [&](std::filesystem::path directory)
    {
        while(this->state == Logging )
        {
            std::cout << "Frame Manager Thread" << std::endl;
            rs2::frameset frame = this->pipe.wait_for_frames();

            if (rs2::frameset fs = frame.as<rs2::frameset>())
            {
                std::cout << "Viedeo Frame Received" << std::endl;
                saveImage(directory, fs, &counter);
            }
            else
            {
                std::unique_lock<std::mutex> lck(this->countMutex) ;
                counters[fs.get_profile().unique_id()]++;
                std::cout << "IMU Data Received" << std::endl;
            }
        }
    };


    std::cout << "Start FrameManager thread" << std::endl;

    this->frameManagerThread = std::thread(frameManager, directory);

    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
};

void CameraManager::stopLogging() {
    this->pipe.stop();
    this->state = Idle;

    this->frameManagerThread.detach();
}

// PRIVATE Functions Implementation

void CameraManager::saveImage(std::filesystem::path dir, rs2::frameset fs, int* counter) {

    rs2::colorizer color_map;
    std::cout << std::to_string(fs.get_timestamp()) << std::endl;

    std::unique_lock<std::mutex> lck(this->countMutex) ;

    for(auto&& frame: fs) {
        if(auto vf = frame.as<rs2::video_frame>()) {
            auto stream = frame.get_profile().stream_type();

            // Add frame to counter
            counters[fs.get_profile().unique_id()]++;

            if(vf.is<rs2::depth_frame>()) vf = color_map.process(frame);


            auto timestamp = vf.get_frame_metadata(RS2_FRAME_METADATA_FRAME_TIMESTAMP);

            // TODO: Change Compression Format
            cv::Mat image(
                cv::Size(vf.get_width(), vf.get_height()),
                CV_8UC3,
                (void*) vf.get_data(),
                cv::Mat::AUTO_STEP
            );



            std::stringstream filepath;
            filepath << dir.string();
            filepath << "image-" << vf.get_profile().stream_name() << "-" << std::to_string(fs.get_timestamp()) << ".png";

            cv::imwrite(filepath.str(), image);
        }

    }
}
