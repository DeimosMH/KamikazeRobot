#include <thread>
#include <chrono>
#include "../include/ev3dev.h"
#include "control.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>

int avg(std::tuple<int, int, int> tuple) {
    return (std::get<0>(tuple) + std::get<1>(tuple) + std::get<2>(tuple)) / 3;
}

int main() {
    /*
     * left_motor
     * right_motor
     * gyro
     * color
     */
     control controller(ev3dev::OUTPUT_D,
                        ev3dev::OUTPUT_A,
                        ev3dev::INPUT_4,
                        ev3dev::INPUT_1);

     controller.drive();

/*    ev3dev::color_sensor left_color_sensor(ev3dev::INPUT_4);
    ev3dev::color_sensor right_color_sensor(ev3dev::INPUT_1);

    while (true) {

        auto left = left_color_sensor.raw(true);
        auto right = right_color_sensor.raw(true);

        std::cout << "left: " << avg(left) << std::endl;
        std::cout << "right: " << avg(right) << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));

    }*/

    return 0;
}
