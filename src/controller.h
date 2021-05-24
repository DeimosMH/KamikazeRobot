#include "../include/ev3dev/ev3dev.h"
#include "engine.h"
#include "communication.h"
#include "detection_system.h"

#ifndef ROBOT_CONTROL_H
#define ROBOT_CONTROL_H

#define FORWARD 0
#define LEFT 90
#define U_TURN 180
#define RIGHT -90

namespace robot {

    /**
     *
     *
     * https://github.com/ddemidov/ev3dev-lang-cpp/blob/master/demos/drive-test.cpp
     *
     * TODO: Add odometry / mapping and coordinates
     */
    class controller {

        static constexpr int TURN_POINT = 1;
        static constexpr int ON_LINE = 2;
        static constexpr int LEFT_ERROR = 3;
        static constexpr int RIGHT_ERROR = 4;
        static constexpr int DEAD_END = 5;


        ev3dev::color_sensor left_color_sensor;
        ev3dev::color_sensor right_color_sensor;

        robot::engine engine;
        robot::communication communication;
        robot::detection_system detection_system;

    public:
        controller();

        [[noreturn]] void drive();

        void print_color();

        void test_comm();

    private:

        const int turn_rates[3] = {FORWARD, LEFT, RIGHT};

        int position_x = 0;
        int position_y = 0;

        int target_position_x = 0;
        int target_position_y = 0;

        int rotation = 0;


        double previous_integral = 0;
        double previous_error = 0;
        std::chrono::time_point<std::chrono::system_clock> previous_time;

        int get_state();

        void adjust();

        static std::string get_color(int color);

        static bool is_white_or_yellow(int left, int right);

        static bool is_black(int left, int right);

        static bool is_white(int left, int right);

        static int avg(std::tuple<int, int, int> &tuple);

        double pid(int input, double dt);

        double get_time_diff();

        void callback_handler(const mqtt::const_message_ptr &msg);

        static int random(int upper_bound);

        static std::vector<std::string> split(std::string &string);

        void update_position(int i);
    };
}
#endif //ROBOT_CONTROL_H
