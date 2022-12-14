#include "test_data.h"
#include "test_network.h"

// TODO: try the tests on the real circuit
void setup(){
    UNITY_BEGIN();
    RUN_TEST(test_data);
    RUN_TEST(test_memory);
    RUN_TEST(test_sd);
    RUN_TEST(test_mqtt);
    UNITY_END();
}

void loop(){}