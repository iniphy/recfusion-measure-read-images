#include "timer.hpp"
#include "utils.hpp"

#include <RecFusion.h>
#include <vector>

using namespace RecFusion;

int main() {
    const int max_num_read_images = 1000;
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
    std::cout << "Found " << num_sensor << " sensors" << std::endl;

    // Opening all available sensors
    std::vector<Sensor*> sensors;
    for (int sensor_id = 0; sensor_id < num_sensor; ++sensor_id) {
        bool opening_success;
        Sensor* sensor;
#ifdef RF_136
        sensor = new Sensor();
        opening_success = sensor->open(sensor_id);
        sensors.push_back(sensor);
#elif RF_220
        sensor = sensor_manager.sensor(sensor_id);
        opening_success = sensor->open();
        sensors.push_back(sensor);
#endif
        if (!opening_success) {
            std::cout << "Failed to open sensor with id=" << sensor_id << std::endl;
        }
    }

    Timer timer;
    std::vector<std::unique_ptr<ColorImage>> color_images;
    std::vector<std::unique_ptr<DepthImage>> depth_images;

    // Creating vectors for keeping time measurements and color/depth image placeholders
    std::vector<std::vector<long long int>> times(num_sensor);
    for (int sensor_id= 0; sensor_id < num_sensor; ++sensor_id) {
        color_images.push_back(std::make_unique<ColorImage>(640, 480));
        depth_images.push_back(std::make_unique<DepthImage>(640, 480));
        times[sensor_id].reserve(max_num_read_images);
    }

    // Measuring the time of reading images (color and depth)
    std::cout << "Performing " << max_num_read_images << " image reads per sensors" << std::endl;
    for (int capture_id = 0; capture_id < max_num_read_images; ++capture_id) {
        for (int sensor_id = 0; sensor_id < num_sensor; ++sensor_id) {
            bool reading_success;
            timer.reset();
            reading_success = sensors[sensor_id]->readImage(*depth_images[sensor_id], *color_images[sensor_id]);
            times[sensor_id].push_back(timer.elapsed());
            if (!reading_success) {
                std::cerr << "Failed to read color and depth frames for sensor with id=" << sensor_id
                          << " at capture id=" << capture_id << std::endl;
            }
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
#ifdef RF_220
        // Quick fix: we need to stop sensors before closing them, otherwise, an error occurs
        // We need to do this even though the Sensor::start() has been never called
        sensors[sensor_id]->stop();
#endif
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