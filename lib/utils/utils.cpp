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
