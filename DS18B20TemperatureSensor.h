#pragma once

#include "indiweather.h"

class DS18B20TemperatureSensor : public INDI::Weather
{
    public:
        DS18B20TemperatureSensor();

        //  Generic indi device entries
        bool Connect() override;
        bool Disconnect() override;
        virtual const char *getDefaultName() override;
        virtual bool initProperties() override;

    protected:
        bool Handshake() override;
        virtual IPState updateWeather() override;
};
