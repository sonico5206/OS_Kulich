#include <iostream>
#include <string>
#include <pthread.h>
#include <stdlib.h>
#include<windows.h> 
#include <vector>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static double sum_of_account;
pthread_t thread1, thread2, thread3;
int status1, status2, status3;
int status_addr1, status_addr2, status_addr3;

bool make_operation(const double& current_operation) {                  // делаем +- операцию
    std::cout << "Start operation: " << current_operation << "\n";
    double temp = sum_of_account + current_operation;
    if (temp < 0) {                                                     // если операция не валидная, сумма меньше 0
        std::cout << "The value is not valid" << std::endl;
        return false;
    }
    int num = 100 + rand() % 200; // 100 - 300 ms duration
    Sleep(num);
    std::cout << "was: " << sum_of_account << " operation: " << current_operation << " stalo: " << temp << std::endl;
    sum_of_account = temp;
    std::cout << "End operation: " << current_operation << "\n";
    return true;
}

void mutexOperation(const double& current_operation) {
    pthread_mutex_lock(&mutex);         //надо чтобы потоки выполнялись по очереди
    make_operation(current_operation);
    pthread_mutex_unlock(&mutex);
}

void* runOperations(void* params) {
    std::vector<double>operations = *(std::vector<double>*)params;
    for (size_t i = 0; i < operations.size(); i++) {
        mutexOperation(operations[i]);
        int num = 100 + rand() % 200;
        Sleep(num);
    }
    return 0;
}

void* getCurrentSum(void* args) {
    while (true) {
        int num = 100 + rand() % 200;
        Sleep(num);
        std::cout << "\nCurrent amount of bank account is: " << sum_of_account << "\n";
    }
    return 0;
}

int main() {
    srand(time(NULL));
    std::cout << "Enter summa of your account:" << std::endl;
    std::cin >> sum_of_account;
    int amountOfOperations1, amountOfOperations2;  //количество операций в каждом потоке
    std::cout << "Enter the amount of operations of first and second threads: " << std::endl;
    std::cin >> amountOfOperations1;
    std::cin >> amountOfOperations2;
    std::vector<double> a1, a2;
    std::cout << "Enter your first " << amountOfOperations1 << " operations: " << std::endl;
    for (int i = 0; i < amountOfOperations1; i++) {     // загоняем в векторы
        double a;
        std::cin >> a;
        a1.push_back(a);
    }
    std::cout << "Enter your second " << amountOfOperations2 << " operations: " << std::endl;
    for (int i = 0; i < amountOfOperations2; i++) {
        double a;
        std::cin >> a;
        a2.push_back(a);
    }

    status1 = pthread_create(&thread1, NULL, runOperations, &a1); // создаем 2 потока для операций  
    status2 = pthread_create(&thread2, NULL, runOperations, &a2);
    status3 = pthread_create(&thread3, NULL, getCurrentSum, NULL);//3ий для выведения суммы 

    status1 = pthread_join(thread1, (void**)&status_addr1);
    status2 = pthread_join(thread2, (void**)&status_addr2);

    std::cout << "Summa of account: " << sum_of_account;
    return 0;
}