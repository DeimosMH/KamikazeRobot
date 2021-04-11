#include <thread>
#include <chrono>
#include <iostream>
#include "control.h"

#define SLEEP_TIME 1
#define TURN_SPEED 100

control::control(const char left_motor_port[],
                 const char right_motor_port[],
                 const char gyro_sensor_port[]) :
        left_motor(left_motor_port),
        right_motor(right_motor_port),
        gyro_sensor(gyro_sensor_port) {
}

void control::turn_left() {
    reset();

    left_motor.set_speed_sp(TURN_SPEED).run_forever();
    right_motor.set_speed_sp(-TURN_SPEED).run_forever();

    while (gyro_sensor.angle() < 90) {
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
    }

    std::cout << gyro_sensor.angle() << std::endl;

    stop();
}

void control::turn_right() {
    reset();

    left_motor.set_speed_sp(-TURN_SPEED).run_forever();
    right_motor.set_speed_sp(TURN_SPEED).run_forever();

    while (gyro_sensor.angle() > -90) {
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
    }

    std::cout << gyro_sensor.angle() << std::endl;

    stop();
}

/**
 * Meh
 * https://github.com/ev3dev/ev3dev/issues/402
 */
void control::reset() {
    gyro_sensor.set_mode(ev3dev::gyro_sensor::mode_gyro_rate);
    gyro_sensor.set_mode(ev3dev::gyro_sensor::mode_gyro_ang);
}

void control::stop() {
    left_motor.stop();
    right_motor.stop();
}
