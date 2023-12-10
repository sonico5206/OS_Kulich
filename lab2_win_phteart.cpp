#include <pthread.h>
#include <Windows.h>
#include <stdio.h>
#include <iostream>

#define SUCCESS 0

void* randomNumber(void* arg) {
    // std::cout << "\nGenerate a new random number [0, 1) every 1 second" << std::endl;

    clock_t TimeZero = clock();

    double deltaTime = 0;
    double secondsToDelay = 1;

    srand(time(0));

    while (1) {
        deltaTime = (clock() - TimeZero) / CLOCKS_PER_SEC;
        //std::cout << "\b" << secondsToDelay - deltaTime << "\b"; // timer

        if (deltaTime > secondsToDelay) {
            double i = rand() / (RAND_MAX + 1.);
            std::cout << "\nNew random : " << i << " \n";

            deltaTime = clock();
            TimeZero = clock();
        }
    }
    return SUCCESS;
}

void* readFromConsole(void* arg) {
    while (1) {
        // std::cout << "\nEnter your string: " << std::endl;
        std::string s;
        std::cin >> s;
        std::reverse(s.begin(), s.end());
        std::cout << "Reversed string: " << s << std::endl;;
    }
    return SUCCESS;
}

BOOL handlerRoutine(DWORD ctrlType) {
    if (ctrlType == CTRL_C_EVENT) {
        return TRUE;
    }
    return FALSE;
}

int main() {
    pthread_t thread1, thread2;
    int status1, status2;
    int status_addr1, status_addr2;

    // create a thread
    status1 = pthread_create(&thread1, NULL, randomNumber, NULL);
    status2 = pthread_create(&thread2, NULL, readFromConsole, NULL);

    // join threads
    status1 = pthread_join(thread1, (void**)&status_addr1);
    status2 = pthread_join(thread2, (void**)&status_addr2);

    // command for getting signal "Ctrl + C"
    SetConsoleCtrlHandler(handlerRoutine, TRUE);
    while (1);

    return 0;
}
