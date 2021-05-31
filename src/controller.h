#include "../include/ev3dev/ev3dev.h"
#include "engine.h"
#include "communication.h"
#include "detection_system.h"

#ifndef ROBOT_CONTROL_H
#define ROBOT_CONTROL_H

namespace robot {

    /**
     * https://github.com/ddemidov/ev3dev-lang-cpp/blob/master/demos/drive-test.cpp
     * http://robotsforroboticists.com/pid-control/
    */
    class controller {

        static constexpr int TURN_POINT = 1;
        static constexpr int ON_LINE = 2;
        static constexpr int OBJECT_FOUND = 3;
        static constexpr int DEAD_END = 5;

        static constexpr int NORTH = 0;
        static constexpr int EAST = 1;
        static constexpr int SOUTH = 2;
        static constexpr int WEST = 3;

        static constexpr int FORWARD = 0;
        static constexpr int LEFT = 90;
        static constexpr int U_TURN = 180;
        static constexpr int RIGHT = -90;

        static constexpr double K_P = 1;
        static constexpr double K_I = 0.01;
        static constexpr double K_D = 2;

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
        int rotation = 0;

        bool has_target = false;
        int target_position_x = 0;
        int target_position_y = 0;


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

        double pid(int left, int right);

        double get_time_diff();


        static int random(int upper_bound);


        void update_position(int turn_angle);

        bool scan = true;
        bool false_alarm = false;


        std::vector<std::pair<int, int>> responses{};

        void potential_enemy_detected();

        int get_direction();

        void handle_dead_end();

        void at_turn_point();

        std::pair<int, int> get_color();

        void update_rotation(int turn_angle);

        void receive_respond_position(std::pair<int, int> target);

        void receive_identify_position();

        void receive_enemy_detected(std::pair<int, int> target);
    };
}
#endif //ROBOT_CONTROL_H
