#include "Sensor.h"
#include "../Server/Server.h"
#include "../DataBackupSd/DataBackupSD.h"
#include "../utils/utils.h"

Data::Data() {
    temperature = 0.0;
    humidity = 0.0;
    pressure = 0.0;
    windSpeed = -1.0;
    windDirection = 0;
    log_file = "/"
}