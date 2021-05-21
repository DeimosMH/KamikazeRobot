#include <thread>
#include <iostream>
#include "controller.h"

#define BLACK 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4
#define RED 5
#define WHITE 6
#define BROWN 7

/*
 * Sauce http://robotsforroboticists.com/pid-control/
 */
//constexpr double K_U = 0.5;
//constexpr double P_U = 40;
constexpr double K_P = 0.3;
constexpr double K_I = 0.05;
constexpr double K_D = 0.025;

robot::controller::controller() :
        left_color_sensor(ev3dev::INPUT_4),
        right_color_sensor(ev3dev::INPUT_1),
        engine()
//        communication([](const mqtt::const_message_ptr &message) {
//            callback_handler(message);
//        }),
{
    previous_time = std::chrono::high_resolution_clock::now();
}

void robot::controller::callback_handler(const mqtt::const_message_ptr &message) {
    std::cout << message->get_payload_str() << std::endl;
    std::cout << message->get_topic() << std::endl;
}

bool robot::controller::is_black(int left, int right) {
    return left == BLACK && right == BLACK;
}

bool robot::controller::is_white(int left, int right) {
    return left == WHITE && right == WHITE;

}

bool robot::controller::is_white_or_yellow(int left, int right) {
    return (left == WHITE || left == YELLOW) && (right == WHITE || right == YELLOW);
}

int robot::controller::get_state() {
    auto left = left_color_sensor.color(true);
    auto right = right_color_sensor.color(true);
    std::cout << "left: " << get_color(left) << std::endl;
    std::cout << "right: " << get_color(right) << std::endl;

    if (is_white(left, right)) return DEAD_END;
    else if (is_black(left, right)) return TURN_POINT;
    else return ON_LINE;
}

[[noreturn]] void robot::controller::drive() {
    while (true) {
        switch (get_state()) {
            case DEAD_END:
                engine.stop();
                engine.turn_left();
                engine.turn_left();
                break;
            case ON_LINE:
                adjust();
                break;
            case TURN_POINT:
                engine.stop();
                engine.turn_left();
                break;
        }
    }
}

void robot::controller::adjust() {
    auto dt = get_time_diff();
    auto left_color = left_color_sensor.raw(true);
    auto left_avg_color = avg(left_color);
    auto right_color = right_color_sensor.raw(true);
    auto right_avg_color = avg(right_color);

    auto adjust = pid(left_avg_color - right_avg_color, dt);

    std::cout
            << "left avg color " << left_avg_color
            << " right avg color " << right_avg_color
            << " speed " << adjust
            << std::endl;

    engine.set_speed(adjust, -adjust);
}

int robot::controller::avg(std::tuple<int, int, int> &tuple) {
    return (std::get<0>(tuple) + std::get<1>(tuple) + std::get<2>(tuple)) / 3;
}

//int robot::controller::proportional(int in) {
//    return P_VALUE * in;
//}

double robot::controller::get_time_diff() {
    auto now = std::chrono::high_resolution_clock::now();
    auto milliseconds = std::chrono::duration<double, std::milli>(now - previous_time);
    previous_time = now;
    return milliseconds.count();
}

double robot::controller::pid(int error, double t) {
    double dt = t / 100;
    double proportional = error;
    double integral = (previous_integral + error);
    double derivative = (error - previous_error);
    previous_error = error;
    previous_integral = integral;

    auto p = K_P * proportional;
    auto i = K_I * integral * dt;
    auto d = K_D * derivative / dt;
    std::cout
            << " K_P " << K_P
            << " K_I " << K_I
            << " K_D " << K_D
            << " dt " << dt
            << " error " << error
            << " p " << p
            << " i " << i
            << " d " << d
            << std::endl;
    return p + i + d;
}

void robot::controller::print_color() {
    auto left = left_color_sensor.color(true);
    auto right = right_color_sensor.color(true);
    std::cout << "left " << get_color(left) << std::endl;
    std::cout << "right " << get_color(right) << std::endl;
}

std::string robot::controller::get_color(int color) {
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

//void robot::controller::test_comm() {
//  communication.send_enemy_detected_message();
//  communication.send_identify_position_message();
//}
