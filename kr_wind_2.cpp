#include <iostream>
#include <string>
#include <pthread.h>
#include <stdlib.h>
#include<windows.h> 
#include <vector>
#include <sstream>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static double amountAtBankAccount;
static int possibleAmountOfRetry = 5; // default value
pthread_t operationsThread1, operationsThread2, readAmountThread;
int operationsStatus1, operationsStatus2, readAmountStatus;
int status_addr1, status_addr2, readAmountAddr;

bool applyOperation(const double& currentAmount) {
	double temp = amountAtBankAccount + currentAmount;
	if (temp < 0) {
		std::cout << "Can't apply operation: " << currentAmount << ", result is negative: " << temp << std::endl;
		return false;
	}
	int num = 100 + rand() % 200; // 25 - 75 ms duration
	Sleep(num);
	std::cout << "was: " << amountAtBankAccount << " operation: " << currentAmount << " became: " << temp << std::endl;
	amountAtBankAccount = temp;
	return true;
}

bool mutexOperation(const double& currentAmount) {
	pthread_mutex_lock(&mutex);
	bool result = applyOperation(currentAmount);
	pthread_mutex_unlock(&mutex);
	return result;
}

void* runOperations(void* params) {
	std::vector<double>operations = *(std::vector<double>*)params;
	int currentTry = 0;
	while (currentTry++ < possibleAmountOfRetry && operations.size() > 0) {
		for (size_t i = 0; i < operations.size(); i++) {
			if (mutexOperation(operations[i])) {
				operations.erase(std::next(operations.begin(), i));
			}
			int num = 100 + rand() % 200;
			Sleep(num);
		}

	}
	return 0;
}

void* getCurrentAmountOfBankAccount(void* args) {
	while (true) {
		int num = 100 + rand() % 200; 
		Sleep(num);
		std::cout << "\nCurrent amount of bank account is: " << amountAtBankAccount << "\n";
	}
	return 0;
}

int main(int argc, char* argv[]) {
	srand(time(NULL));

	if (argc < 7) {
		std::cout << "Not enough parameters of command line.\n Please, use correct flags to run program.\n";
		return -1;
	}


	std::vector<double> operationValues1, operationValues2;

	for (size_t i = 1; i < argc; i++)
	{
		if (std::string(argv[i]) == "-v1") {
			std::string val = std::string(argv[i + 1]);

			std::istringstream iss = std::istringstream{ val };
			double t;
			while (iss >> t) {
				operationValues1.push_back(t);
			}
		}

		if (std::string(argv[i]) == "-v2") {
			std::string val = std::string(argv[i + 1]);

			std::istringstream iss = std::istringstream{ val };
			double t;
			while (iss >> t) {
				operationValues2.push_back(t);
			}
		}

		if (std::string(argv[i]) == "-a") {
			amountAtBankAccount = std::stod(std::string(argv[i + 1]));
		}

		if (std::string(argv[i]) == "-r") {
			possibleAmountOfRetry = std::atoi(argv[i + 1]);
		}
	}

	operationsStatus1 = pthread_create(&operationsThread1, NULL, runOperations, &operationValues1);
	operationsStatus2 = pthread_create(&operationsThread2, NULL, runOperations, &operationValues2);
	readAmountStatus = pthread_create(&readAmountThread, NULL, getCurrentAmountOfBankAccount, NULL);

	operationsStatus1 = pthread_join(operationsThread1, (void**)&status_addr1);
	operationsStatus2 = pthread_join(operationsThread2, (void**)&status_addr2);

	std::cout << "Your bank account: " << amountAtBankAccount << std::endl;
	return 0;
}