#include <iostream>
#include "detection_system.h"

robot::detection_system::detection_system() : ultrasonic_sensor{ev3dev::INPUT_3} {

}

bool robot::detection_system::scan() {
    auto u = ultrasonic_sensor.distance_centimeters(true);
    std::cout << "u " << u << std::endl;
    return u < 10;
}
