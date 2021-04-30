#include <thread>
#include <chrono>
#include <iostream>
#include "control.h"

#define SLEEP_TIME 100
#define TURN_SPEED 100
#define TURN_NINETY_DEGREES 220
#define P_VALUE 0.5
#define BLACK 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4
#define RED 5
#define WHITE 6
#define BROWN 7
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
    left_motor
            .set_position_sp(-TURN_NINETY_DEGREES)
            .set_speed_sp(TURN_SPEED)
            .run_to_rel_pos();
    right_motor
            .set_position_sp(TURN_NINETY_DEGREES)
            .set_speed_sp(TURN_SPEED)
            .run_to_rel_pos();

    while (left_motor.state().count("running") ||
           right_motor.state().count("running")) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void control::turn_right() {

    left_motor.set_speed_sp(-TURN_SPEED).run_forever();
    right_motor.set_speed_sp(TURN_SPEED).run_forever();
    stop();
}

void control::stop() {
    left_motor.stop();
    right_motor.stop();
}

bool control::is_black(int left, int right) {
    return left == BLACK && right == BLACK;
}

bool control::is_white(int left, int right) {
    return left == WHITE && right == WHITE;

}

bool control::is_white_or_yellow(int left, int right) {
    return (left == WHITE || left == YELLOW) && (right == WHITE || right == YELLOW);
}

int control::get_state() {
    auto left = left_color_sensor.color(true);
    auto right = right_color_sensor.color(true);
    std::cout << "left: " << get_color(left) << std::endl;
    std::cout << "right: " << get_color(right) << std::endl;

    if (is_white(left, right)) return DEAD_END;
    else if (is_black(left, right)) return TURN_POINT;
    else return ON_LINE;
}

[[noreturn]] void control::drive() {
    while (true) {
        switch (get_state()) {
            case DEAD_END:
                stop();
                turn_left();
                turn_left();
                break;
            case ON_LINE:
                adjust();
                break;
            case TURN_POINT:
                stop();
                turn_left();
                break;
        }
    }
}

void control::adjust() {
    std::cout << "left" << std::endl;
    set_speed(left_motor, left_color_sensor);

    std::cout << "right" << std::endl;
    set_speed(right_motor, right_color_sensor);

    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
}

int control::avg(std::tuple<int, int, int> &tuple) {
    return (std::get<0>(tuple) + std::get<1>(tuple) + std::get<2>(tuple)) / 3;
}

int control::proportional(int in) {
    return P_VALUE * in;
}

void control::set_speed(ev3dev::large_motor &engine, ev3dev::color_sensor &color_sensor) {
    auto color = color_sensor.raw(true);
    auto avg_color = avg(color);
    auto speed = proportional(avg_color);
    std::cout << "avg color " << avg_color << " speed " << speed << std::endl;
    engine.set_speed_sp(std::min(speed, ENGINE_MAX_SPEED)).run_forever();
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
}

void control::print_color() {
    auto left = left_color_sensor.color(true);
    auto right = right_color_sensor.color(true);
    std::cout << "left " << get_color(left) << std::endl;
    std::cout << "right " << get_color(right) << std::endl;
}

std::string control::get_color(int color) {
    switch (color) {
        case 0:
            return "No color";
        case BLACK:
            return "Black";
        case BLUE:
            return "Blue";
        case GREEN:
            return "Green";
        case YELLOW:
            return "Yellow";
        case RED:
            return "Red";
        case WHITE:
            return "White";
        case BROWN:
            return "Brown";
        default:
            throw std::runtime_error("Illegal Argument");
    }
}
