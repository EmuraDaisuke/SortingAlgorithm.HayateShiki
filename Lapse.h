#pragma once



#include <chrono>
#include <iostream>
#include <iomanip>



// 

class Lapse final {
    public:
        ~Lapse();
        Lapse();
    
    private:
        double Now();
        double mLapse;
};



// 

Lapse::~Lapse()
{
    std::cout << std::fixed << std::setprecision(8) << (Now() - mLapse) << std::endl;
}



Lapse::Lapse()
:mLapse(Now())
{
}



double
Lapse::Now()
{
    using namespace std::chrono;
    return static_cast<double>(duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count()) / 1000000000;
}
