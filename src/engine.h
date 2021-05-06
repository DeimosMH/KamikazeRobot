#include "../include/ev3dev/ev3dev.h"
#include <iostream>

#ifndef ROBOT_ENGINE_H
#define ROBOT_ENGINE_H

namespace robot {

    class engine {

        ev3dev::large_motor left_motor;
        ev3dev::large_motor right_motor;

    public:
        engine();

        void turn_right();

        void turn_left();

        void stop();

        void set_speed(int left_correction, int right_correction);
    };
}
#endif //ROBOT_ENGINE_H
