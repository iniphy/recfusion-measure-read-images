//
// Created by quepas on 09/08/2022.
//

#ifndef RF_MEASURE_READ_IMAGES_PROJECT_UTILS_HPP
#define RF_MEASURE_READ_IMAGES_PROJECT_UTILS_HPP

#include <algorithm>
#include <numeric>
#include <vector>

template<typename T>
T MeanValue(const std::vector<T>& numbers) {
    T sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    return sum / numbers.size();
}

template<typename T>
T MinValue(const std::vector<T>& numbers) {
    return *std::min_element(numbers.begin(), numbers.end());
}

template<typename T>
T MaxValue(const std::vector<T>& numbers) {
    return *std::max_element(numbers.begin(), numbers.end());
}

#endif //RF_MEASURE_READ_IMAGES_PROJECT_UTILS_HPP
