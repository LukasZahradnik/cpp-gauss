#include "timer.h"
#include <iostream>
#include <chrono>

using namespace std;

Timer::Timer(const char *header) {
    this->header = header;
    this->_beg = _clock::now();
}

Timer::~Timer() {
    auto _end = _clock::now();
    cout << this->header
         << chrono::duration_cast<chrono::nanoseconds>(_end - this->_beg).count()
         << " nanoseconds" << endl;
}


