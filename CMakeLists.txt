project(indi_ds18b20_temperature_sensor C CXX)
cmake_minimum_required(VERSION 3.1)

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

find_package(INDI REQUIRED)

# these will be used to set the version number in config.h and our driver's xml file
set(CDRIVER_VERSION_MAJOR 1)
set(CDRIVER_VERSION_MINOR 0)

# do the replacement in the config.h
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

# do the replacement in the driver's xml file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/indi_ds18b20_temperature_sensor.xml.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/indi_ds18b20_temperature_sensor.xml
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${INDI_INCLUDE_DIR})

add_executable(indi_ds18b20_temperature_sensor DS18B20TemperatureSensor.cpp)
target_link_libraries(indi_ds18b20_temperature_sensor ${INDI_LIBRARIES})

# tell cmake where to install our executable
install(TARGETS indi_ds18b20_temperature_sensor RUNTIME DESTINATION bin)

# and where to put the driver's xml file.
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/indi_ds18b20_temperature_sensor.xml
    DESTINATION ${INDI_DATA_DIR}
)