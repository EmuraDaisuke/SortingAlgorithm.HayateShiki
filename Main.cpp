


#include <cassert>
#include <random>
#include <vector>
#include <array>
#include <algorithm>
#include <iostream>
#include <memory.h>

#if USE_GFX_TIMSORT//[
#include "./timsort.hpp"
#endif//]

#include "./HayateShiki.h"
#include "./Lapse.h"



using sort_t = float;

int precision(int v){ return 1; }
float precision(float v){ return (v/0x00ffffff); }
double precision(double v){ return (v/0x000fffffffffffff); }



enum eSrc {
    Rand,
    Flat,
    Inc,
    Dec,
    IBin,
    IInc,   // interleaved increasing
    IDec,   // interleaved decreasing
    
    Num,
};

static const char* apSrc[eSrc::Num]={
    "Rand",
    "Flat",
    "Inc",
    "Dec",
    "IBin",
    "IInc",
    "IDec",
};



#define ORDER   0
#define FAT     0

struct Test
{
    sort_t m;
    #if ORDER//[
    std::size_t o;
    #endif//]
    #if FAT//[
    std::size_t f[FAT];
    #endif
};

bool operator <(const Test& s, const Test& t)
{
    return s.m < t.m;
}

bool operator ==(const Test& s, const Test& t)
{
    return s.m == t.m;
}



void dump(std::vector<Test>& a)
{
    for (auto& v : a) std::cout << v.m << std::endl;
    std::cout << std::endl;
}

void increment(std::vector<Test>& a)
{
    sort_t m = 1;
    for (auto& v : a){ v.m = m; m += precision(m); }
}

void decrement(std::vector<Test>& a)
{
    sort_t m = 1;
    for (auto& v : a){ v.m = -m; m += precision(m); }
}

void interleave(std::vector<Test>& a)
{
    auto n = a.size() & ~1;
    for (auto o = n-n; o < n; o += 2) std::swap(a[o], a[o+1]);
}

void init(eSrc Src, std::vector<Test>& a, std::mt19937& rRand, std::uniform_int_distribution<>& rRange)
{
    {   // 
        std::size_t o = 0;
        for (auto& v : a){
            #if ORDER//[
            v.o = o++;
            #endif//]
            #if FAT//[
            for (auto& f : v.f) f = 0;
            #endif//]
        }
    }
    
    {   // 
        switch (Src){
            default:
            case eSrc::Rand:{
                for (auto& v : a) v.m = rRange(rRand);
                break;
            }
            case eSrc::Flat:{
                sort_t m = 1;
                for (auto& v : a) v.m = m;
                break;
            }
            case eSrc::Inc:{
                increment(a);
                break;
            }
            case eSrc::Dec:{
                decrement(a);
                break;
            }
            case eSrc::IBin:{
                bool b = false;
                for (auto& v : a){ v.m = (b)? 0:1; b = !b; }
                break;
            }
            case eSrc::IInc:{
                increment(a);
                interleave(a);
                break;
            }
            case eSrc::IDec:{
                decrement(a);
                interleave(a);
                break;
            }
        }
    }
}



void test(eSrc Src, int nTest, int nLoop)
{
    std::random_device Seed;
    std::mt19937 Rand(Seed());
    std::uniform_int_distribution<> Range(0, std::numeric_limits<int>::max());
    auto a = std::vector<Test>(nTest);
    
    printf("\n--- %s %d\n", apSrc[Src], nTest);
    
    #if 1//[
    {   // 
        double t0 = 0;
        double t1 = 0;
        double t2 = 0;
        double t3 = 0;
        
        for (auto n = nLoop; n; --n){
            init(Src, a, Rand, Range);
            
            #if 1//[
            {   // 
                auto s = a;
                auto l = Lapse::Now();
                std::sort(s.begin(), s.end());
                t0 += Lapse::Now() - l;
            }
            #endif//]
            
            #if 1//[
            {   // 
                auto s = a;
                auto l = Lapse::Now();
                std::stable_sort(s.begin(), s.end());
                t1 += Lapse::Now() - l;
            }
            #endif//]
            
            #if USE_GFX_TIMSORT//[
            {   // 
                auto s = a;
                auto l = Lapse::Now();
                gfx::timsort(s.begin(), s.end());
                t2 += Lapse::Now() - l;
            }
            #endif//]
            
            #if 1//[
            {   // 
                auto s = a;
                auto l = Lapse::Now();
                HayateShiki::sort(s.begin(), s.end());
                t3 += Lapse::Now() - l;
            }
            #endif//]
        }
        
        printf("std::sort         : "); Lapse::Out(t0 / nLoop);
        printf("std::stable_sort  : "); Lapse::Out(t1 / nLoop);
        #if USE_GFX_TIMSORT//[
        printf("gfx::timsort      : "); Lapse::Out(t2 / nLoop);
        #endif//]
        printf("HayateShiki::sort : "); Lapse::Out(t3 / nLoop);
    }
    #else//][
    for (auto n = nLoop; n; --n){
        init(Src, a, Rand, Range);
        
        auto s0 = a;
        auto s1 = a;
        auto s2 = a;
        
        std::sort(s0.begin(), s0.end());
        std::stable_sort(s1.begin(), s1.end());
        HayateShiki::sort(s2.begin(), s2.end());
        
        auto bEqual01 = (s0 == s1);
        auto bEqual12 = (s1 == s2);
        auto bEqual20 = (s2 == s0);
        
        #if 0//[
        printf("\n");
        printf("%d %d\n", bEqual01, (a == s0));
        printf("%d %d\n", bEqual12, (a == s1));
        printf("%d %d\n", bEqual20, (a == s2));
        #endif//]
        assert(bEqual12);
    }
    #endif//]
}



int main(int argc, char* argv[])
{
    test(eSrc::Rand,     10000, 1000);
    test(eSrc::Rand,   1000000, 100);
    test(eSrc::Rand, 100000000, 10);
    
    test(eSrc::Flat, 100000000, 5);
    test(eSrc::Inc,  100000000, 5);
    test(eSrc::Dec,  100000000, 5);
    test(eSrc::IBin, 100000000, 5);
    test(eSrc::IInc, 100000000, 5);
    test(eSrc::IDec, 100000000, 5);
    return 0;
}
