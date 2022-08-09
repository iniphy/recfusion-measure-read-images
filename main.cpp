#define RF_220
#include "timer.hpp"
#include <iostream>

#include <RecFusion.h>
#include <vector>
#include <memory>

using namespace RecFusion;

int main() {
    const int max_num_read_images = 100;
    int num_sensor;

#ifdef RF_136
    std::cout << "Using RecFusion 1.3.6 backend" << std::endl;
    Sensor query_sensor;
    num_sensor = query_sensor.deviceCount();
#elifdef RF_220
    RecFusionSDK::init();
    std::cout << "Using RecFusion 2.2.0 backend" << std::endl;
    SensorManager sensor_manager;
    num_sensor = sensor_manager.deviceCount();
#endif

    // Opening all available sensors
    std::vector<std::unique_ptr<Sensor>> sensors(num_sensor);
    for (int sensor_id = 0; sensor_id < num_sensor; ++sensor_id) {
#ifdef RF_136
        sensors.push_back(std::make_unique<Sensor>());
        sensors[sensor_id]->open(sensor_id);
#elifdef RF_220
        sensors.push_back(sensor_manager.sensor(sensor_id))
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
    for (int capture_id = 0; capture_id < 100; ++capture_id) {
        for (int sensor_id = 0; sensor_id < num_sensor; ++sensor_id) {
            timer.reset();
            sensors[sensor_id]->readImages(*depth_img, *color_img);
            times[sensor_id].push_back(timer.elapsed());
        }
    }

    // Closing sensors
    for (int sensor_id = 0; sensor_id < num_sensor; ++sensor_id) {
        sensors[sensor_id]->close();
    }

#ifdef RF_220
    RecFusionSDK::deinit();
#endif
}