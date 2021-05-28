#include "../include/ev3dev/ev3dev.h"
#include <iostream>

#ifndef ROBOT_ENGINE_H
#define ROBOT_ENGINE_H

namespace robot {

    class engine {

        static const int STEP_FORWARD_ROTATION;
        static const int TURN_NINETY_DEGREES;
        static const int ENGINE_MAX_SPEED;
        static const int BASE_SPEED;
        static const int WAIT_TIME_MILLISECONDS;

    private:
        ev3dev::gyro_sensor gyro_sensor;
        ev3dev::large_motor left_motor;
        ev3dev::large_motor right_motor;


    public:
        engine();

        void turn_left();

        /**
         * Stop both engines immediately
         */
        void stop();

        void set_speed(double left_correction, double right_correction);

        void turn(int degrees);

        void move();
    };
}
#endif //ROBOT_ENGINE_H
