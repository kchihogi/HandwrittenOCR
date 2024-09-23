cmake_minimum_required(VERSION 3.10)
project(HandwrittenOCR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(OpenCV REQUIRED)
find_package(Tesseract REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${Tesseract_INCLUDE_DIRS})

add_executable(ocr_executable src/ocr.cpp)
target_link_libraries(ocr_executable ${OpenCV_LIBS} tesseract)
