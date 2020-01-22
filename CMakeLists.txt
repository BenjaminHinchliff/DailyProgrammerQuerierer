cmake_minimum_required(VERSION 3.8)

project("DailyProgrammer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(dp "main.cpp")

# add cpr to package
find_package(ZLIB REQUIRED)
target_link_libraries(dp PRIVATE ZLIB::ZLIB)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(dp PRIVATE cpr)

# add json handling
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(dp PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

# add program options from boost
find_package(Boost 1.48 REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(dp PRIVATE ${Boost_LIBRARIES})
