#include "../include/ev3dev/ev3dev.h"
#include "control.h"
#include "communication.h"

int main() {

    communication communication("tcp://test.mosquitto.org:1883", "ev3dev-emil");

    communication.send();

    control controller(ev3dev::OUTPUT_D, // left motor
                       ev3dev::OUTPUT_A, // right motor
                       ev3dev::INPUT_4,  // left color
                       ev3dev::INPUT_1); // right color

//    controller.drive();
    /*for (int i = 0; i < 10; ++i) {
        controller.print_color();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }*/

    std::this_thread::sleep_for(std::chrono::seconds(20));


    return 0;
}
