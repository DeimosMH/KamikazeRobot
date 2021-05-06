#include "controller.h"

int main() {


    robot::controller controller;
    controller.test_comm();

    /*for (int i = 0; i < 10; ++i) {
        controller.print_color();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }*/

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true)
        std::this_thread::sleep_for(std::chrono::seconds(1));
#pragma clang diagnostic pop


    return 0;
}
