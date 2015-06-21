#include <iostream>
#include <cstdlib>
#include <chrono>

int main(int argc, char *argv[]) {
    srand48(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::cout << "Hello, World!" << std::endl;

    return 0;
}
