#include "timer.hpp"
#include "utils.hpp"

#include <RecFusion.h>
#include <vector>

using namespace RecFusion;

int main() {
    const int max_num_read_images = 100;
    int num_sensor = 0;

#ifdef RF_136
    std::cout << "Using RecFusion 1.3.6 backend" << std::endl;
    Sensor query_sensor;
    num_sensor = query_sensor.deviceCount();
#elif RF_220
    std::cout << "Using RecFusion 2.2.0 backend" << std::endl;
    RecFusionSDK::init();
    SensorManager sensor_manager;
    num_sensor = sensor_manager.deviceCount();
#endif

    // Opening all available sensors
    std::vector<Sensor*> sensors(num_sensor);
    for (int sensor_id = 0; sensor_id < num_sensor; ++sensor_id) {
#ifdef RF_136
        sensors.push_back(new Sensor());
        sensors[sensor_id]->open(sensor_id);
#elif RF_220
        sensors.push_back(sensor_manager.sensor(sensor_id));
        sensors[sensor_id]->open();
#endif
    }

    Timer timer;
    std::unique_ptr<ColorImage> color_img;
    std::unique_ptr<DepthImage> depth_img;

    // Creating vectors for keeping time measurements
    std::vector<std::vector<long long int>> times(num_sensor);
    for (int sensor_id= 0; sensor_id < num_sensor; ++sensor_id) {
        times[sensor_id].reserve(max_num_read_images);
    }

    // Measuring the time of reading images (color and depth)
    std::cout << "Performing " << max_num_read_images << " image reads per sensors" << std::endl;
    for (int capture_id = 0; capture_id < max_num_read_images; ++capture_id) {
        for (int sensor_id = 0; sensor_id < num_sensor; ++sensor_id) {
            timer.reset();
            sensors[sensor_id]->readImage(*depth_img, *color_img);
            times[sensor_id].push_back(timer.elapsed());
        }
    }

    // Displaying measurements
    std::cout << "=== Measurements ===" << std::endl;
    for (int sensor_id = 0; sensor_id < num_sensor; ++sensor_id) {
        std::cout << "sensor_id=" << sensor_id <<
                     "; min=" << MinValue(times[sensor_id]) << "ms" <<
                     "; mean=" << MeanValue(times[sensor_id]) << "ms" <<
                     "; max=" << MaxValue(times[sensor_id]) << "ms" << std::endl;
    }

    // Closing sensors
    for (int sensor_id = 0; sensor_id < num_sensor; ++sensor_id) {
        sensors[sensor_id]->close();
    }

#ifdef RF_136
    for (int sensor_id = 0; sensor_id < num_sensor; ++sensor_id) {
        delete sensors[sensor_id];
    }
#elif RF_220
    RecFusionSDK::deinit();
#endif
}