#include "test_network.h"

#define LEN_MESSAGE_BYTES 4
byte *data_received = nullptr;



void callback(const char *topic, byte *payload, unsigned int length) {

    // allocates the memory(to be freed) to read the message
    data_received = (byte *) malloc(sizeof(byte) * (length + 4));

    uint_to_bytes(payload, length, LEN_MESSAGE_BYTES); // encodes the length in the message

    for(int i=0; i< length; i++)
        data_received[i+4] = payload[i];

}

void test_mqtt(){
    Info cred = Info();

    WiFiClient espClient;
    PubSubClient client = PubSubClient(espClient);
    unsigned int length;
    char *message;

    init_wifi(cred.ssid.c_str(), cred.password.c_str());
    init_client(cred.mqtt_server.c_str(), cred.mqtt_port, callback, client);
    delay(1000);
    client_publish("dummy_test_net", "DUMMY_DATA", client);
    client_read("dummy_test_net", client); // this executes the callback

    TEST_ASSERT_NOT_EQUAL(nullptr, data_received);

    bytes_to_uint(data_received, length, LEN_MESSAGE_BYTES); // we get back the length of the message into length
    message = (char*) malloc(sizeof(char) * (length+1));
    TEST_ASSERT_NOT_EQUAL(NULL, message);

    bytes_to_string(data_received+LEN_MESSAGE_BYTES, message, length);
    TEST_ASSERT_EQUAL_STRING("DUMMY_DATA", message); // -->  this is the actual comparison

    free(message);
    free(data_received);
}
