cmake_minimum_required(VERSION 3.1)

PROJECT(indi_ds18b20_temperature_sensor VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")
set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")

set(PROGRAM_PERMISSIONS_DEFAULT
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

find_package(INDI REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${INDI_INCLUDE_DIR})

add_executable(indi_ds18b20_temperature_sensor DS18B20TemperatureSensor.cpp)
target_link_libraries(indi_ds18b20_temperature_sensor ${INDI_LIBRARIES} pthread ${EXTRA_LIBS})

install(TARGETS indi_ds18b20_temperature_sensor RUNTIME DESTINATION bin
    PERMISSIONS ${PROGRAM_PERMISSIONS_DEFAULT} SETUID)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/indi_ds18b20_temperature_sensor.xml" DESTINATION ${INDI_DATA_DIR})
