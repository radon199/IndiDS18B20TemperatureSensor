#include "DS18B20TemperatureSensor.h"

#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include <memory>

#include <iostream>

// We declare a pointer to DS18B20TemperatureSensor.
std::unique_ptr<DS18B20TemperatureSensor> ds18b20TemperatureSensor(new DS18B20TemperatureSensor);

DS18B20TemperatureSensor::DS18B20TemperatureSensor()
{
    std::cout << "created" << std::endl;
    setVersion(1, 0);
}

const char *DS18B20TemperatureSensor::getDefaultName()
{
    return "DS18B20 Temperature Sensor";
}

bool DS18B20TemperatureSensor::Connect()
{
    std::cout << "test" << std::endl;
    return true;
}

bool DS18B20TemperatureSensor::Disconnect()
{
    return true;
}

bool DS18B20TemperatureSensor::Handshake()
{
    return true;
}

bool DS18B20TemperatureSensor::initProperties()
{
    INDI::Weather::initProperties();

    addParameter("WEATHER_TEMPERATURE", "Temperature (C)", -20, 40, 15);

    addAuxControls();

    return true;
}

IPState DS18B20TemperatureSensor::updateWeather()
{
    // From https://github.com/rkaczorek/astroberry-diy/blob/master/astroberry_focuser.cpp
	DIR *dir;
	struct dirent *dirent;
	char dev[16];            // Dev ID
	char devPath[128];       // Path to device
	char buf[256];           // Data from device
	char temperatureData[6]; // Temp C * 1000 reported by device
	char path[] = "/sys/bus/w1/devices";
	ssize_t numRead;

	dir = opendir(path);

	// search for --the first-- DS18B20 device
	if (dir != NULL)
	{
		while ((dirent = readdir(dir)))
		{
			// DS18B20 device is family code beginning with 28-
			if (dirent->d_type == DT_LNK && strstr(dirent->d_name, "28-") != NULL)
			{
				strcpy(dev, dirent->d_name);
				break;
			}
		}
		closedir(dir);
	} else {
		DEBUG(INDI::Logger::DBG_WARNING, "Temperature sensor disabled. 1-Wire interface is not available.");
		return IPS_ALERT;
	}

	// Assemble path to --the first-- DS18B20 device
	sprintf(devPath, "%s/%s/w1_slave", path, dev);

	// Opening the device's file triggers new reading
	int fd = open(devPath, O_RDONLY);
	if(fd == -1)
	{
		DEBUG(INDI::Logger::DBG_WARNING, "Temperature sensor not available.");
		return IPS_ALERT;
	}

	// read sensor output
	while((numRead = read(fd, buf, 256)) > 0);
	close(fd);

	// parse temperature value from sensor output
	strncpy(temperatureData, strstr(buf, "t=") + 2, 5);
	DEBUGF(INDI::Logger::DBG_DEBUG, "Temperature sensor raw output: %s", buf);
	DEBUGF(INDI::Logger::DBG_DEBUG, "Temperature string: %s", temperatureData);

	double tempC = strtod(temperatureData, NULL) / 1000;

	// check if temperature is reasonable
	if(abs(tempC) > 100)
	{
		DEBUG(INDI::Logger::DBG_DEBUG, "Temperature reading out of range.");
		return IPS_ALERT;
	}

    setParameterValue("WEATHER_TEMPERATURE", tempC);

    return IPS_OK;
}
