#include <Windows.h>
#include <iostream>
#include <ctime>
#include <thread>
#include <deque>
#include <mutex>
using namespace std;

int nps = 0;
deque<int> histnps (60);
mutex deq;

void draw() {
	HDC hdc = GetDC(NULL);
	for (;;) {
		deq.lock();
		for (int i = 0; i < histnps.size(); i++) {
			SetPixel(hdc, i + 1000, 60 - histnps[i], RGB(255, 255, 255));
		}
		Sleep(1);
		deq.unlock();
	}
	Sleep(1);
}

int main() {
	HDC hdc = GetDC(NULL);

	for (int i = 0; i < 60; i++) {
		histnps.push_back(0);
	}

	thread draw(draw);
	bool pressed[4] = { false, false, false, false, };
	long int sec = static_cast<long int> (time(0));
	int nps = 0;

	while (true) {
		if (GetAsyncKeyState('S')) {
			if (pressed[0] == false) {
				nps++;
				pressed[0] = true;
			}
		}
		else {
			pressed[0] = false;
		}

		if (GetAsyncKeyState('D')) {
			if (pressed[1] == false) {
				nps++;
				pressed[1] = true;
			}
		}
		else {
			pressed[1] = false;
		}

		if (GetAsyncKeyState('K')) {
			if (pressed[2] == false) {
				nps++;
				pressed[2] = true;
			}
		}
		else {
			pressed[2] = false;
		}

		if (GetAsyncKeyState('L')) {
			if (pressed[3] == false) {
				nps++;
				pressed[3] = true;
			}
		}
		else {
			pressed[3] = false;
		}

		if (static_cast<long int> (time(0)) > sec) {
			cout << nps << endl;
			deq.lock();
			histnps.pop_front();
			histnps.push_back(nps);
			deq.unlock();
			for (int j = 1120; j > 1000; j--) {
				for (int i = 60; i > 10; i--) {
					SetPixel(hdc, j, i, RGB(0, 0, 0));
				}
			}
			nps = 0;
			sec = static_cast<long int> (time(0));
		}

		Sleep(1);
	}
	draw.join();
}