#include <thread>
#include <iostream>
#include <random>
#include <sstream>
#include "controller.h"

#define BLACK 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4
#define RED 5
#define WHITE 6
#define BROWN 7

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define FORWARD 0
#define LEFT 90
#define U_TURN 180
#define RIGHT -90

/*
 * Sauce http://robotsforroboticists.com/pid-control/
 */
//constexpr double K_U = 0.5;
//constexpr double P_U = 40;
constexpr double K_P = 1;
//constexpr double K_I = 0.05;
constexpr double K_I = 0;

constexpr double K_D = 1;


robot::controller::controller() : left_color_sensor{ev3dev::INPUT_4},
                                  right_color_sensor{ev3dev::INPUT_1},
                                  engine{},
                                  communication{[this](const mqtt::const_message_ptr &msg) {
                                      callback_handler(msg);
                                  }, 1},
                                  detection_system{} {
    previous_time = std::chrono::high_resolution_clock::now();
}

void robot::controller::callback_handler(const mqtt::const_message_ptr &msg) {

    if (msg->get_topic() == "ev3dev/robot/enemy-detected") {

        std::string a = msg->get_payload();
        auto vec = split(a);
        target_position_x = std::stoi(vec[0]);
        target_position_y = std::stoi(vec[1]);
        std::cout << "x " << target_position_x << "y " << target_position_y << std::endl;

    } else if (msg->get_topic() == "ev3dev/robot/identify-position") {

        communication.send_respond_position_message(position_x, position_y);

    } else if (msg->get_topic() == "ev3dev/robot/respond-position") {

        std::string a = msg->get_payload();
        auto vec = split(a);
        int x = std::stoi(vec[0]);
        int y = std::stoi(vec[1]);
        std::cout << "x " << x << "y " << y << std::endl;
    }
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

int robot::controller::get_state()  {
    auto left = left_color_sensor.color(true);
    auto right = right_color_sensor.color(true);
//    std::cout << "left: " << get_color(left) << std::endl;
//    std::cout << "right: " << get_color(right) << std::endl;

    if (is_white(left, right)) return DEAD_END;
    else if (is_black(left, right)) return TURN_POINT;
    else return ON_LINE;
}

[[noreturn]] void robot::controller::drive() {
    if (detection_system.scan()) {
        communication.send_identify_position_message();
    }
    while (true) {
        switch (get_state()) {
            case DEAD_END:
                engine.stop();
                engine.turn(180);
                break;
            case ON_LINE:
                adjust();
                break;
            case TURN_POINT:
                engine.stop();
                auto d = turn_rates[random(3)];
                update_position(d);
                engine.turn(d);
                engine.move();
                break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void robot::controller::adjust() {
    auto left_color = left_color_sensor.raw(true);
    auto left_avg_color = avg(left_color);
    auto right_color = right_color_sensor.raw(true);
    auto right_avg_color = avg(right_color) + 10; // right measures too low add 10

    auto adjust = pid(left_avg_color, right_avg_color);

//    std::cout
//            << "left avg color " << left_avg_color
//            << " right avg color " << right_avg_color
//            << " speed " << adjust
//            << std::endl;

    engine.set_speed(adjust, -adjust);
}

int robot::controller::avg(std::tuple<int, int, int> &tuple) {
    return (std::get<0>(tuple) + std::get<1>(tuple) + std::get<2>(tuple)) / 3;
}

double robot::controller::get_time_diff() {
    auto now = std::chrono::high_resolution_clock::now();
    auto milliseconds = std::chrono::duration<double, std::milli>(now - previous_time);
    previous_time = now;
    return milliseconds.count() / 100.0;
}

double robot::controller::pid(int left,
                              int right) {
    auto error = left - right;
    auto dt = get_time_diff();
    auto integral = previous_integral + error;
    auto derivative = error - previous_error;
    previous_error = error;
    previous_integral = integral;

    auto p = K_P * error;
    auto i = K_I * integral * dt;
    auto d = K_D * derivative / dt;
//    std::cout
//            << " K_P " << K_P
//            << " K_I " << K_I
//            << " K_D " << K_D
//            << " dt " << dt
//            << " error " << error
//            << " p " << p
//            << " i " << i
//            << " d " << d
//            << std::endl;
    auto speed = p + i + d;
    speed = std::min(speed, 500.0);
    speed = std::max(speed, -500.0);
    return speed;
}

void robot::controller::print_color() {
    auto left_color = left_color_sensor.raw(true);
    auto left_avg_color = avg(left_color);

    auto right_color = right_color_sensor.raw(true);
    auto right_avg_color = avg(right_color) + 10;

    auto left = left_color_sensor.color(true);
    auto right = right_color_sensor.color(true);

    std::cout << "left " << get_color(left) << std::endl;
    std::cout << "right " << get_color(right) << std::endl;
    std::cout << "left avg color " << left_avg_color << std::endl;
    std::cout << "right avg color " << right_avg_color << std::endl;
    std::cout << "diff color " << right_avg_color - left_avg_color << std::endl;

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

void robot::controller::test_comm() {
    communication.send_enemy_detected_message();
    communication.send_identify_position_message();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantParameter"
int robot::controller::random(int upper_bound) {
    std::default_random_engine generator; // NOLINT(cert-msc51-cpp)
    std::uniform_int_distribution<int> distribution{0, upper_bound};
    return distribution(generator);
}
#pragma clang diagnostic pop

std::vector<std::string> robot::controller::split(const std::string &string) {
    std::stringstream string_stream{string};
    std::vector<std::string> result;
    while (string_stream.good()) {
        std::string substr;
        getline(string_stream, substr, ',');
        result.push_back(substr);
    }
    return result;
}

void robot::controller::update_position(int turn_angle) {
    switch (turn_angle) {
        case LEFT:
            rotation = (rotation + 1) % 4;
            break;
        case RIGHT:
            rotation = (rotation - 1) % 4;
            break;
        case FORWARD:
            break;
        case U_TURN:
            rotation = (rotation + 2) % 4;
            break;
        default:
            throw std::runtime_error("Illegal argument");
    }
    switch (rotation) {
        case NORTH:
            position_y++;
            break;
        case EAST:
            position_x++;
            break;
        case SOUTH:
            position_y--;
            break;
        case WEST:
            position_x--;
            break;
    }
}
