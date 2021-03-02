#include <thread>
#include <chrono>
#include "ev3dev.h"
#include "control.h"

int main() {
    control controller(ev3dev::OUTPUT_A,
                       ev3dev::OUTPUT_D,
                       ev3dev::INPUT_1);

    controller.turn_left();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    controller.turn_right();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    controller.stop();

    return 0;
}
