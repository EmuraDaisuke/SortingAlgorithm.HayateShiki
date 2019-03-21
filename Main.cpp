


#include <vector>
#include <random>
#include <algorithm>
#include <memory.h>

#include "./HayateShiki.h"
#include "./Lapse.h"



enum class eSrc {
    Rand,
    Inc,
    Dec,
    Flat,
};

using sort_t = float;



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



void test(eSrc Src, int nTest, int nRange, int nLoop, int nRepeat)
{
    std::random_device Seed;
    std::mt19937 Rand(Seed());
    std::uniform_int_distribution<> Range(0, nRange);
    
    auto a = std::vector<Test>(nTest);
    for (auto n = nLoop; n; --n){
        printf("\n\n--- %d\n", nTest);
        
        switch (Src){
            case eSrc::Rand:{
                for (auto& v : a) v.m = Range(Rand);
                break;
            }
            case eSrc::Inc:{
                sort_t m = 0;
                for (auto& v : a) v.m = ++m;
                break;
            }
            case eSrc::Dec:{
                sort_t m = 0;
                for (auto& v : a) v.m = --m;
                break;
            }
            case eSrc::Flat:{
                sort_t m = 0;
                for (auto& v : a) v.m = m;
                break;
            }
        }
        
        {   // 
            std::size_t o = 0;
            for (auto& v : a){
                #if ORDER//[
                v.o = ++o;
                #endif//]
                #if FAT//[
                for (auto& f : v.f) f = 0;
                #endif//]
            }
        }
        
        #if 1//[
        printf("\n== std::sort\n");
        for (auto n = nRepeat; n; --n){
            auto s = a;
            Lapse l;
            std::sort(s.begin(), s.end());
        }
        #endif//]
        
        #if 1//[
        printf("\n== std::stable_sort\n");
        for (auto n = nRepeat; n; --n){
            auto s = a;
            Lapse l;
            std::stable_sort(s.begin(), s.end());
        }
        #endif//]
        
        #if 1//[
        printf("\n== HayateShiki::Sort\n");
        for (auto n = nRepeat; n; --n){
            auto s = a;
            Lapse l;
            HayateShiki::Sort(s.data(), s.size());
        }
        #endif//]
        
        #if 0//[
        {   // 
            auto s0 = a;
            auto s1 = a;
            auto s2 = a;
            
            std::sort(s0.begin(), s0.end());
            std::stable_sort(s1.begin(), s1.end());
            HayateShiki::Sort(s2.data(), s2.size());
            
            auto bStrict01 = (memcmp(s0.data(), s1.data(), s1.size() * sizeof(Test)) == 0);
            auto bStrict12 = (memcmp(s1.data(), s2.data(), s2.size() * sizeof(Test)) == 0);
            auto bStrict20 = (memcmp(s2.data(), s0.data(), s0.size() * sizeof(Test)) == 0);
            
            printf("\n");
            printf("%d %d %d\n", (s1 == s0), bStrict01, (a == s0));
            printf("%d %d %d\n", (s2 == s1), bStrict12, (a == s1));
            printf("%d %d %d\n", (s0 == s2), bStrict20, (a == s2));
        }
        #endif//]
    }
}



int main(int argc, char* argv[])
{
    eSrc Src = eSrc::Rand;
    int nRange = std::numeric_limits<int>::max();
    test(Src,     10000, nRange, 1, 4);
    test(Src,   1000000, nRange, 1, 4);
    test(Src, 100000000, nRange, 1, 4);
    return 0;
}
