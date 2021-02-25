#include <iostream>
#include "ev3dev.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto tech = ev3dev::power_supply::battery.technology();
    std::cout << tech << std::endl;

    std::cout << "Hello, World! again" << std::endl;
    return 0;
}
