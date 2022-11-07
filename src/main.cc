#include <librealsense2/rs.hpp>

#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    rs2::context ctx;

    std::cout << "hello from librealsense - " << RS2_API_VERSION_STR << std::endl;
    std::cout << "You have " << ctx.query_devices().size() << " RealSense devices connected" << std::endl;

    rs2::colorizer color_map;

    rs2::pipeline pipe;
    pipe.start();

    // Skip first 30 frames to let auto exposure settle
    for(auto i = 0; i < 30; i++) pipe.wait_for_frames();


    for(auto&& frame: pipe.wait_for_frames()) {
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
            filepath << "image-" << vf.get_profile().stream_name() << ".png";

            cv::imwrite(filepath.str(), image);
        }

    }


    return 0;
}

