#include<iostream>
#include <windows.h>
#include <conio.h>
#include <signal.h>
using namespace std;

void SignalHandler(int signal)
{
	abort();
}

int main()
{
	int sig = 13;    // Enter
	while (true)
	{
		if (_kbhit()) // слушатель нажатия на клаву
		{
			char c = _getch();

			if (int(c) == sig) {
				cout << c;
				SignalHandler(0);
			}
		}
	}
}
