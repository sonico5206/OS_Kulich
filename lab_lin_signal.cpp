#include <iostream>
#include <csignal>
#include <unistd.h>

void signalHandler(int signal) {
    std::cout << "Received signal: " << signal << std::endl;
    exit(signal);
}

int main() {
    signal(SIGINT, signalHandler);

    std::cout << "Waiting for signal..." << std::endl;

    while (true) {
        sleep(1);
    }

    return 0;
}