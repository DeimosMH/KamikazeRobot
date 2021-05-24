//
// Created by emil on 5/24/21.
//

#include "detection_system.h"

robot::detection_system::detection_system() : ultrasonic_sensor{ev3dev::INPUT_3} {

}

bool robot::detection_system::scan() {
    return ultrasonic_sensor.distance_centimeters(true) < 10;
}
