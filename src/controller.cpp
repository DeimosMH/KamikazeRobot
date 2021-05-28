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

robot::controller::controller() :
        left_color_sensor{ev3dev::INPUT_4},
        right_color_sensor{ev3dev::INPUT_1},
        engine{},
        communication{
                [this](const std::pair<int, int> &target) {
                    receive_enemy_detected(target);
                },
                [this]() {
                    receive_identify_position();
                },
                [this](const std::pair<int, int> &target) {
                    receive_respond_position(target);
                }
        },
        detection_system{} {
    previous_time = std::chrono::high_resolution_clock::now();
}

void robot::controller::receive_respond_position(const std::pair<int, int> &target) {
    responses.push_back(target);
}

void robot::controller::receive_identify_position() {
    communication.send_respond_position_message(position_x, position_y);
}

void robot::controller::receive_enemy_detected(const std::pair<int, int> &target) {
    target_position_x = target.first;
    target_position_y = target.second;
    has_target = true;
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
    const auto color = get_color();
    if (scan && detection_system.scan()) return OBJECT_FOUND;
    else if (is_white(color.first, color.second)) return DEAD_END;
    else if (is_black(color.first, color.second)) return TURN_POINT;
    else return ON_LINE;
}

std::pair<int, int> robot::controller::get_color() {
    return {
            left_color_sensor.color(true),
            right_color_sensor.color(true)
    };
}

[[noreturn]] void robot::controller::drive() {
    while (true) {
        switch (get_state()) {
            case OBJECT_FOUND:
                potential_enemy_detected();
                break;
            case DEAD_END:
                handle_dead_end();
                break;
            case ON_LINE:
                adjust();
                break;
            case TURN_POINT:
                at_turn_point();
                break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void robot::controller::potential_enemy_detected() {
    communication.send_identify_position_message();
    scan = false;
    engine.stop();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    for (auto &t : responses) {
        if (t.first == position_x && t.second == position_y)
            false_alarm = true;
    }
    if (!false_alarm) communication.send_enemy_detected_message(position_x, position_y);
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

double robot::controller::pid(int left, int right) {
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
    communication.send_enemy_detected_message(1, 1);
    communication.send_identify_position_message();
}

int robot::controller::random(int upper_bound) {
    std::random_device random_device; // create object for seeding
    std::mt19937 engine{random_device()};
    std::uniform_int_distribution<int> distribution{0, upper_bound};
    return distribution(engine);
}

void robot::controller::update_position(int turn_angle) {
    update_rotation(turn_angle);
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

int robot::controller::get_direction() {
    const auto dx = target_position_x - position_x;
    const auto dy = target_position_y - position_y;

    switch (rotation) {
        case NORTH:
            if (dy > 0) return FORWARD;
            else if (dy < 0) return U_TURN;
            else if (dx > 0) return RIGHT;
            else if (dx < 0) return LEFT;
            else break;

        case EAST:
            if (dy > 0) return FORWARD;
            else if (dy < 0) return RIGHT;
            else if (dx > 0) return LEFT;
            else if (dx < 0) return U_TURN;
            else break;

        case SOUTH:
            if (dy > 0) return U_TURN;
            else if (dy < 0) return FORWARD;
            else if (dx > 0) return LEFT;
            else if (dx < 0) return RIGHT;
            else break;

        case WEST:
            if (dy > 0) return U_TURN;
            else if (dy < 0) return LEFT;
            else if (dx > 0) return RIGHT;
            else if (dx < 0) return FORWARD;
            else break;

    }
    engine.stop();
    exit(0); /* Explode xD */
}

void robot::controller::handle_dead_end() {
    update_rotation(U_TURN);
    engine.turn(U_TURN);
}

void robot::controller::at_turn_point() {
    engine.stop();
    int d;
    if (has_target) d = get_direction();
    else d = turn_rates[random(3)];
    update_position(d);
    std::cout << "d " << d << std::endl;
    engine.turn(d);
    engine.move();
}

void robot::controller::update_rotation(int turn_angle) {
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
}
