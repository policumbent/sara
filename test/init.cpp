#include "test_data.h"

// TODO: try the tests on the real circuit
void setup(){
    UNITY_BEGIN();
    RUN_TEST(test_data);
    RUN_TEST(test_memory);
    RUN_TEST(test_sd);
    UNITY_END();
}


void loop(){}