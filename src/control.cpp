#include <thread>
#include <chrono>
#include <iostream>
#include "control.h"

#define SLEEP_TIME 100
#define TURN_SPEED 100
#define TURN_NINETY_DEGREES 260
#define P_VALUE 0.5
#define BLACK 1
#define ENGINE_MAX_SPEED 1050

control::control(const char left_motor_port[],
                 const char right_motor_port[],
                 const char left_color_sensor_port[],
                 const char right_color_sensor_port[]) :
        left_motor(left_motor_port),
        right_motor(right_motor_port),
        left_color_sensor(left_color_sensor_port),
        right_color_sensor(right_color_sensor_port) {
}

void control::turn_left() {
    left_motor.set_position_sp(TURN_NINETY_DEGREES).set_speed_sp(500).run_to_rel_pos();
    right_motor.set_position_sp(-TURN_NINETY_DEGREES).set_speed_sp(500).run_to_rel_pos();
}

void control::turn_right() {
    reset();

    left_motor.set_speed_sp(-TURN_SPEED).run_forever();
    right_motor.set_speed_sp(TURN_SPEED).run_forever();


    stop();
}

/**
 * Meh
 * https://github.com/ev3dev/ev3dev/issues/402
 */
void control::reset() {
}

void control::stop() {
    left_motor.stop();
    right_motor.stop();
}

int control::getState() {
    bool left = left_color_sensor.color(true) == BLACK;
    bool right = right_color_sensor.color(true) == BLACK;
    std::cout << "left: " << left << std::endl;
    std::cout << "right: " << right << std::endl;

    if (!left && !right) {
        return ON_LINE;
    } else if (left && !right) {
        return LEFT_ERROR;
    } else if (!left && right) {
        return RIGHT_ERROR;
    } else {
        return TURN_POINT;
    }
}

[[noreturn]] void control::drive() {

    while (true) {
        switch (getState()) {
            case ON_LINE:
                adjust();
                break;
            case RIGHT_ERROR:
                adjust();
                break;
            case LEFT_ERROR:
                adjust();
                break;
            case TURN_POINT:
                stop();
                break;
        }

    }
}


void control::adjust() {
    std::cout << "left" << std::endl;
    set_speed(left_motor,left_color_sensor);

    std::cout << "right" << std::endl;
    set_speed(right_motor,right_color_sensor);

    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
}

int control::avg(std::tuple<int, int, int> &tuple) {
    return (std::get<0>(tuple) + std::get<1>(tuple) + std::get<2>(tuple)) / 3;
}

int control::proportional(int in) {
    return P_VALUE * in;
}

void control::set_speed(ev3dev::large_motor &engine, ev3dev::color_sensor &color_sensor) {
    auto color = right_color_sensor.raw(true);
    auto avg_color = avg(color);
    auto speed = proportional(avg_color);
    std::cout << "avg color " << avg_color << " speed " << speed << std::endl;
    right_motor.set_speed_sp(std::min(speed, ENGINE_MAX_SPEED)).run_forever();
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
}



