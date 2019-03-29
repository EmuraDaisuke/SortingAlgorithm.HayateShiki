#pragma once



#include <chrono>
#include <iostream>
#include <iomanip>



// 

class Lapse final {
    public:
        ~Lapse();
        Lapse();
        static double Now();
        static void Out(double Time);
    
    private:
        double mLapse;
};



// 

Lapse::~Lapse()
{
    Out(Now() - mLapse);
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



void
Lapse::Out(double Time)
{
    std::cout << std::fixed << std::setprecision(8) << Time << std::endl;
}
