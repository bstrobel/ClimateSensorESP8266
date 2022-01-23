/*
 * ----------------------------------------------------------------------------------------------------
 * Project Name : Climate Sensor V1.00
 * MCDisplay.h - Initializes the Display and provides methods to display the measurements
 * ----------------------------------------------------------------------------------------------------
 */

#ifndef MCDISPLAY_H
#define MCDISPLAY_H
#include <exception>
#include <stdexcept>
#include <string>
#include "MeasureClimate.h"

class MCDisplay final
{
public:
    MCDisplay();
    ~MCDisplay();
    void enable();
    void disable();
    void displayWeatherData(MeasureClimate &mc);
    void displayError(const std::string &who, const std::string &what);
    const std::string toString();
private:
    std::string sensor_hostname;
};
#endif // MCDISPLAY_H