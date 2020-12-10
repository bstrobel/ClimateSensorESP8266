#include <string>
#include <sstream>
#include <Adafruit_BME280.h>
#include "MeasureClimate.h"

using namespace std;

MeasureClimate::MeasureClimate(float elevation)
{
    getDatafromBME280();
    calcDewpointHumidityAdj();
    calcPressureRel(elevation);
};

void MeasureClimate::getDatafromBME280()
{
    Adafruit_BME280 bme;
    if (!bme.begin(0x76))   //address either 0x76 or 0x77
    {
        throw MeasurementException("Could not find a valid BME280 sensor, check wiring!");
    }

    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1, // temperature
                    Adafruit_BME280::SAMPLING_X1, // pressure
                    Adafruit_BME280::SAMPLING_X1, // humidity
                    Adafruit_BME280::FILTER_OFF   );

    bme.takeForcedMeasurement();
    MeasureClimate::temperature = bme.readTemperature();
    MeasureClimate::humidity = bme.readHumidity();
    MeasureClimate::pressure_abs = bme.readPressure() / 100.0F;
};

void MeasureClimate::calcDewpointHumidityAdj()
{
    float a = 17.271;
    float b = 237.7;
    float tempcalc = (a * temperature) / (b + temperature) + log(humidity*0.01);
    dewpoint_temp = (b * tempcalc) / (a - tempcalc);

    //August-Roche-Magnus approximation (http://bmcnoldy.rsmas.miami.edu/Humidity.html)
    humidity_adj = 100.0F * (exp((a * dewpoint_temp) / (b + dewpoint_temp)) / exp((a * temperature) / (b + temperature)));
    if (humidity_adj > 100.0) humidity_adj = 100.0;    // just in case
};

void MeasureClimate::calcPressureRel(float elevation)
{
    pressure_rel = (((pressure_abs * 100.0F)/pow(1.0F-(elevation/44330.0F), 5.255F))/100.0F);
};

const string MeasureClimate::toString()
{
    stringstream s;
    s << "Temperature: " << temperature << "°C" << endl;
    s << "Dew Point: " << dewpoint_temp << "°C" << endl;
    s << "Humidity: " << humidity << "%" << endl;
    s << "Humidity adjusted: " << humidity_adj << "%" << endl;
    s << "Air Pressure absolute: " << pressure_abs << "hPa" << endl;
    s << "Air Pressure relative: " << pressure_rel << "hPa" << endl;
    return s.str();
}