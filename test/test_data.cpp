#include "test_data.h"
#include "Data.h"
#include "DataBackupFlash.h"
#include "DataBackupSD.h"
#include <ctime>

const unsigned int cs_sd = 2;

void test_data(){
    Data data = Data();

    TEST_ASSERT_EQUAL_DOUBLE(0.0, data.temperature);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, data.humidity);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, data.pressure);
    TEST_ASSERT_EQUAL_DOUBLE(-1.0, data.wind_speed);
    TEST_ASSERT_EQUAL_DOUBLE(-1.0, data.wind_direction);
    TEST_ASSERT_EQUAL_STRING("/", data.log_file.c_str());
}

void test_memory(){
    Data data = Data();
    FlashHandler flash = FlashHandler(data);
    String txt;
    char cmp[256];

    data.timestamp = data.ntp_timestamp = data.gps_timestamp = DateTime(time(nullptr));
    data.log_file = "test_sara_write_flash";
    // flash.flush(); --> be careful to export before testing

    sprintf(cmp, "%02d/%02d/%02d %02d:%02d:%02d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f",
            data.timestamp.year(),data.timestamp.month(), data.timestamp.day(),
            data.timestamp.hour(), data.timestamp.minute(), data.timestamp.second(),

            data.temperature, data.humidity, data.pressure, data.wind_speed, data.wind_direction,
            data.longitude, data.latitude, data.altitude);

    flash.write_flash(data, FILE_WRITE);
    flash.read_flash(data, txt);

    TEST_ASSERT_EQUAL_STRING(cmp, txt.c_str());
}

void test_sd(){
    Data data = Data();
    SDHandler sd = SDHandler(data, cs_sd);
    String txt;
    char cmp[256];

    data.timestamp = data.ntp_timestamp = data.gps_timestamp = DateTime(time(nullptr));
    data.log_file = "test_sara_write_sd";
    sprintf(cmp, "%02d/%02d/%02d %02d:%02d:%02d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f",
            data.timestamp.year(),data.timestamp.month(), data.timestamp.day(),
            data.timestamp.hour(), data.timestamp.minute(), data.timestamp.second(),

            data.temperature, data.humidity, data.pressure, data.wind_speed, data.wind_direction,
            data.longitude, data.latitude, data.altitude);


    sd.write_sd(data, FILE_WRITE);
    sd.read_sd(data, txt);

    TEST_ASSERT_EQUAL_STRING(cmp, txt.c_str());
}