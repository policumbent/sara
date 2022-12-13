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
    data.timestamp = data.ntp_timestamp = data.gps_timestamp = DateTime(time(nullptr));
    data.log_file = "test_sara_write";
    flash.write_flash(data);
}

void test_sd(){
    Data data = Data();
    SDHandler sd = SDHandler(data, cs_sd);
    data.timestamp = data.ntp_timestamp = data.gps_timestamp = DateTime(time(nullptr));
    data.log_file = "test_sara_write";
    sd.write_sd(data);
}