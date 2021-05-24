#include "../include/ev3dev/ev3dev.h"


#ifndef ROBOT_DETECTION_SYSTEM_H
#define ROBOT_DETECTION_SYSTEM_H

namespace robot {

    class detection_system {

        ev3dev::ultrasonic_sensor ultrasonic_sensor;

    public:
        detection_system();

        bool scan();

    };
}

#endif //ROBOT_DETECTION_SYSTEM_H
