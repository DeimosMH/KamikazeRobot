
#include "../include/ev3dev/ev3dev.h"

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

    static constexpr int TURN_POINT = 1;
    static constexpr int ON_LINE = 2;
    static constexpr int LEFT_ERROR = 3;
    static constexpr int RIGHT_ERROR = 4;
    static constexpr int DEAD_END = 5;


    ev3dev::large_motor left_motor;
    ev3dev::large_motor right_motor;
    ev3dev::color_sensor left_color_sensor;
    ev3dev::color_sensor right_color_sensor;

    control(const char left_motor_port[],
            const char right_motor_port[],
            const char left_color_sensor_port[],
            const char right_color_sensor_port[]);

    void turn_left();

    void turn_right();

    void stop();

    [[noreturn]] void drive();

    void print_color();


private:

    int get_state();

    void adjust();

    static std::string get_color(int color);

    static void set_speed(ev3dev::large_motor &engine, ev3dev::color_sensor &color_sensor);

    static bool is_white_or_yellow(int left, int right);

    static bool is_black(int left, int right);
    static bool is_white(int left, int right);

    static int avg(std::tuple<int, int, int> &tuple);

    static int proportional(int in);
};

#endif //ROBOT_CONTROL_H
