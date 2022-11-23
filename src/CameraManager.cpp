#include "../include/CameraManager.h"
#include <cstdlib>

using namespace cm;

CameraManager::CameraManager() {
    this->ctx = rs2::context();
    this->pipe = rs2::pipeline();

    std::cout << "hello from librealsense - " << RS2_API_VERSION_STR << std::endl;
    std::cout << "You have " << ctx.query_devices().size() << " RealSense devices connected" << std::endl;

    //this->pipe.start();
    // Skip first 30 frames to let auto exposure settle
    //for(auto i = 0; i < 30; i++) this->pipe.wait_for_frames();

    //this->pipe.stop();
}

int CameraManager::startLogging(std::filesystem::path directory, rs2::frame *frames) try
{
    int counter = 0;

    rs2::config cfg;

    cfg.enable_stream(RS2_STREAM_COLOR, 1280, 720, RS2_FORMAT_BGR8,30);
    cfg.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);

    auto callback = [&](const rs2::frame& frame)
    {
        // std::lock_guard<std::mutex> lock(mutex);
        rs2::colorizer color_map;

        if (rs2::frameset fs = frame.as<rs2::frameset>())
        {
            std::cout << counter << "- Video Frame Received" << std::endl;

            for(auto&& frame: fs) {
                if(auto vf = frame.as<rs2::video_frame>()) {
                    auto stream = frame.get_profile().stream_type();

                    if(vf.is<rs2::depth_frame>()) vf = color_map.process(frame);

                    // Create OpenCV Mat from RealSense frame data
                    cv::Mat image(
                        cv::Size(vf.get_width(), vf.get_height()),
                        CV_8UC3,
                        (void*) vf.get_data(),
                        cv::Mat::AUTO_STEP
                    );



                    std::stringstream filepath;
                    filepath << directory.string();
                    filepath << "image-" << vf.get_profile().stream_name() << "-" << counter << ".png";

                    cv::imwrite(filepath.str(), image);
                }   

            }
        }
        else
        {
            //std::cout << "IMU Data Received" << std::endl;
        }
        counter++;
    };

    this->pipe.start(callback);

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
}
