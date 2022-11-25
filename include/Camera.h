#include <librealsense2/rs.hpp>

namespace cm {
    class Camera {
        private:
            rs2::device camera;

        public:
            // Initialize a camera with default values
            Camera(rs2::device);

            void printSensorsNames();
    };
}
