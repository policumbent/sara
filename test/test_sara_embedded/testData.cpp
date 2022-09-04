#include <gtest/gtest.h>
#include "Data.h"

TEST(data_test, check_data_init_zero){
    Data data = Data();

    String s("/");
    int comp = -1;

    ASSERT_FLOAT_EQ(0.0, data.temperature)  << "The Data() object was not declared successfully";
    ASSERT_FLOAT_EQ(0.0, data.humidity)  << "The Data() object was not declared successfully";
    ASSERT_FLOAT_EQ(0.0, data.pressure)  << "The Data() object was not declared successfully";
    ASSERT_DOUBLE_EQ(0.0, data.windSpeed)  << "The Data() object was not declared successfully";
    ASSERT_DOUBLE_EQ(0.0, data.windDirection)  << "The Data() object was not declared successfully";

    comp = data.log_file.compareTo(s);

    ASSERT_EQ(0, comp)  << "The Data() object was not declared successfully";
}