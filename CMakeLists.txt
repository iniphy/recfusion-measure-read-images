cmake_minimum_required(VERSION 3.0)

project(rf-measure-read-images-project VERSION "1.0.0" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

set (RECFUSION_136_PATH "C:\\Program Files\\ImFusion\\RecFusionSDK 1.3.6")
set (RECFUSION_220_PATH "C:\\Program Files\\ImFusion\\RecFusion SDK 2.2.0")

set(COMMON_SRC main.cpp timer.hpp)

# RF_136 target
add_executable(rf136-measure-test ${COMMON_SRC})
target_compile_definitions(rf136-measure-test PUBLIC RF_136)
target_include_directories(rf136-measure-test BEFORE PUBLIC ${RECFUSION_136_PATH}/include)
target_link_directories(rf136-measure-test BEFORE PUBLIC ${RECFUSION_136_PATH}/lib)
target_link_libraries(rf136-measure-test RecFusion)

# RF_220 target
add_executable(rf220-measure-test ${COMMON_SRC})
target_compile_definitions(rf220-measure-test PUBLIC RF_220)
target_include_directories(rf220-measure-test BEFORE PUBLIC ${RECFUSION_220_PATH}/include)
target_link_directories(rf220-measure-test BEFORE PUBLIC ${RECFUSION_220_PATH}/lib)
target_link_libraries(rf220-measure-test RecFusionSDK)
