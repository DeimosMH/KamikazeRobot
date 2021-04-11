
#include "../include/ev3dev.h"

#ifndef ROBOT_CONTROL_H
#define ROBOT_CONTROL_H

/**
 *
 *
 * https://github.com/ddemidov/ev3dev-lang-cpp/blob/master/demos/drive-test.cpp
 *
 * TODO: dont really need gyro to turn, see above
 */
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
