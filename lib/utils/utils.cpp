# include "utils.h"

void loop_infinite(){
    while (true){
        delay(100);
    }
}

void get_str(double val, char buff[8]){
    dtostrf(val, 1, 2, buff);
}


void get_str(int val, char buff[8]){
    String(val).toCharArray(buff, 8);
}

// creates 4 8-bit-chunks to encode the length of the message in the payload
// the encoding is performed in LSB format
void uint_to_bytes(uint8_t *payload, unsigned int var, unsigned int len){
    unsigned int mask = 0b00000000000000000000000011111111;
    for(int i=0; i<len;i++){
        payload[i] = mask & (var >> (8*i));
    }
}

// decodes the 4 8-bit-chunks to encode the length of the message in the payload
// the encoding is performed in LSB format
void bytes_to_uint(uint8_t *payload, unsigned int &var, unsigned int len){

    for(unsigned int i=len-1, k=0; i>=0; i--, k++){
        var += payload[i];
        var = var << (8*k);
    }

}

void bytes_to_string(uint8_t *payload, char *message, unsigned int len){
    int i;
    for(i=0; i<len; i++){
        message[i] = (char) payload[i];
    }
    message[i] = '\0'; // line termination must be always included
}
