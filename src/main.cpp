#include <iostream>
#include <cstdlib>
#include <chrono>

#include <simulator.h>

int main(int argc, char *argv[]) {
    srand48(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    simulator s(3, 5);
    s.start();

    return 0;
}
