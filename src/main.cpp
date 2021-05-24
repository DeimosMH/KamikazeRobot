#include "controller.h"

//void handler(int sig) {
//    void *array[10];
//    size_t size;
//
//    // get void*'s for all entries on the stack
//    size = backtrace(array, 10);
//
//    // print out all the frames to stderr
//    fprintf(stderr, "Error: signal %d:\n", sig);
//    backtrace_symbols_fd(array, size, STDERR_FILENO);
//    exit(1);
//}


int main() {

    //signal(SIGSEGV, handler);   // install our handler


    robot::controller controller;
    controller.drive();

//    for (int i = 0; i < 10; ++i) {
//        controller.print_color();
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }


//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "EndlessLoop"
//    while (true) { std::this_thread::sleep_for(std::chrono::seconds(1)); }
//#pragma clang diagnostic pop

    return 0;
}
