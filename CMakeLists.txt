cmake_minimum_required(VERSION 3.8)

project("DailyProgrammer")

add_executable(dp "main.cpp")

# add cpr to package
find_package(ZLIB REQUIRED)
target_link_libraries(dp PRIVATE ZLIB::ZLIB)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(dp PRIVATE cpr)

# add json handling
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(dp PRIVATE nlohmann_json nlohmann_json::nlohmann_json)