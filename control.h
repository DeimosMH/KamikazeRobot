//
// Created by emil on 3/2/21.
//

#include "ev3dev.h"

#ifndef ROBOT_CONTROL_H
#define ROBOT_CONTROL_H


class control {

public:
    ev3dev::large_motor left_motor;
    ev3dev::large_motor right_motor;
    ev3dev::gyro_sensor gyro_sensor;

    control(const char left_motor_port[],
            const char right_motor_port[],
            const char gyro_sensor_port[]);

    void turn_left();

    void turn_right();

    void stop();

private:

    void reset();
};

#endif //ROBOT_CONTROL_H
