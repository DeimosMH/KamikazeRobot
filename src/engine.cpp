
#include <thread>
#include "engine.h"

#define TURN_SPEED 100
#define TURN_NINETY_DEGREES 220
#define ENGINE_MAX_SPEED 1050

robot::engine::engine() :
        left_motor(ev3dev::OUTPUT_D),
        right_motor(ev3dev::OUTPUT_A) {
}

void robot::engine::turn_left() {
    left_motor.set_position_sp(-TURN_NINETY_DEGREES)
            .set_speed_sp(TURN_SPEED)
            .run_to_rel_pos();
    right_motor.set_position_sp(TURN_NINETY_DEGREES)
            .set_speed_sp(TURN_SPEED)
            .run_to_rel_pos();

    while (left_motor.state().count("running") ||
           right_motor.state().count("running")) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void robot::engine::turn_right() {
    left_motor.set_speed_sp(-TURN_SPEED).run_forever();
    right_motor.set_speed_sp(TURN_SPEED).run_forever();
    stop();
}

void robot::engine::stop() {
    left_motor.stop();
    right_motor.stop();
}

void robot::engine::set_speed(int left_correction, int right_correction) {
    auto left_speed = left_correction + ENGINE_MAX_SPEED / 2;
    auto right_speed = right_correction + ENGINE_MAX_SPEED / 2;
    left_motor.set_speed_sp(std::min(left_speed, ENGINE_MAX_SPEED)).run_forever();
    right_motor.set_speed_sp(std::min(right_speed, ENGINE_MAX_SPEED)).run_forever();
    std::cout << "left_correction speed " << left_speed << std::endl;
    std::cout << "right_correction speed " << right_speed << std::endl;
}
