#pragma once
// Copyright(c) 2018 Emura Daisuke
// MIT License



#include <cstddef>
#include <utility>
#include <malloc.h>

#if _MSC_VER//[
// msvc
#define alloca      _alloca
#elif __clang__//][
// clang
#else//][
// gcc
#endif//]

#define Auto                decltype(auto)
#define local_alloc(T, s)   static_cast<T*>(alloca(s))
#define local_array(T, n)   local_alloc(T, sizeof(T) * n)



namespace HayateShiki {
namespace Private {



// 

template <class T>
struct Part
{
    enum oUnit {
        oUnit_Asc,
        oUnit_Dsc,
        oUnit_Num,
    };
    
    T*              a[oUnit_Num];
    std::size_t     n[oUnit_Num];
    oUnit           o;
};



template <class T>
struct Unit
{
    T*              a;
    std::size_t     n;
};



template <class T>
struct Dive
{
    Unit<T>         mUnit;
    T*              mpJoin;
};



// 

inline int Msb(std::size_t v)
{
    if (v){
        union IEEE_754 {
            float v;
            struct {
                unsigned int fraction:23;
                unsigned int exponent:8;
                unsigned int sign:1;
            } map;
        };
        IEEE_754 lzc;
        lzc.v = v;
        return (lzc.map.exponent - 127);
    }
    return -1;
}



inline int MsbAlignment(std::size_t v)
{
    return Msb(v + v - 1);
}



inline int LowerLimit(int v, int limit)
{
    return (v > limit)? v: limit;
}



constexpr std::size_t Bit(int v)
{
    return (1ULL << v);
}



// 

constexpr std::size_t bIns = 5;
constexpr std::size_t nIns = Bit(bIns);



// 

template <class T> std::size_t Num(T* a, T* e);

template <class T> T* Copy(T* pDst, T* pSrc, std::size_t nSrc);
template <class T> T* Copy(T* pDst, T* pSrc, std::size_t nSrc, T& v);

template <class T> T* Join(T* pJoin, Unit<T>* pUnit, Part<T>* pPart);
template <class T> T* Join(T* aJoin, Unit<T>* pUnit, Part<T>* pPart0, Part<T>* pPart1);
template <class T> T* Join(T* aJoin, Unit<T>* pUnit, Unit<T>* pUnit0, Unit<T>* pUnit1);

template <class T> T* InitPart(Part<T>* pPart, T* pSrc, T* eSrc, T** paDsc);



// 

template <class T>
std::size_t Num(T* a, T* e)
{
    return (e - a);
}



template <class T>
T* Copy(T* pDst, T* pSrc, std::size_t nSrc)
{
    while (nSrc--) *pDst++ = std::move(*pSrc++);
    return pDst;
}



template <class T>
T* Copy(T* pDst, T* pSrc, std::size_t nSrc, T& v)
{
    *pDst = std::move(v);
    return Copy(++pDst, ++pSrc, --nSrc);
}



template <class T>
T* Join(T* pJoin, Unit<T>* pUnit, Part<T>* pPart)
{
    auto nDsc = pPart->n[Part<T>::oUnit_Dsc];
    auto nAsc = pPart->n[Part<T>::oUnit_Asc];
    
    pUnit->a = pJoin;
    pUnit->n = nDsc + nAsc;
    
    pJoin = Copy(pJoin, pPart->a[Part<T>::oUnit_Dsc], nDsc);
    pJoin = Copy(pJoin, pPart->a[Part<T>::oUnit_Asc], nAsc);
    return pJoin;
}



template <class T>
T* Join(T* aJoin, Unit<T>* pUnit, Part<T>* pPart0, Part<T>* pPart1)
{
    auto pJoin = aJoin;
    auto o0 = pPart0->o;
    auto o1 = pPart1->o;
    auto p0 = pPart0->a[o0];
    auto p1 = pPart1->a[o1];
    auto n0 = pPart0->n[o0];
    auto n1 = pPart1->n[o1];
    auto v0 = std::move(*p0);
    auto v1 = std::move(*p1);
    
    while (true){
        if (v1 < v0){
            *pJoin++ = std::move(v1);
            if (--n1){
                v1 = std::move(*++p1);
            } else {
                if (o1){
                    o1 = Part<T>::oUnit_Asc;
                    p1 = pPart1->a[o1];
                    n1 = pPart1->n[o1];
                    v1 = std::move(*p1);
                } else {
                    pJoin = Copy(pJoin, p0, n0, v0);
                    if (o0) pJoin = Copy(pJoin, pPart0->a[Part<T>::oUnit_Asc], pPart0->n[Part<T>::oUnit_Asc]);
                    break;
                }
            }
        } else {
            *pJoin++ = std::move(v0);
            if (--n0){
                v0 = std::move(*++p0);
            } else {
                if (o0){
                    o0 = Part<T>::oUnit_Asc;
                    p0 = pPart0->a[o0];
                    n0 = pPart0->n[o0];
                    v0 = std::move(*p0);
                } else {
                    pJoin = Copy(pJoin, p1, n1, v1);
                    if (o1) pJoin = Copy(pJoin, pPart1->a[Part<T>::oUnit_Asc], pPart1->n[Part<T>::oUnit_Asc]);
                    break;
                }
            }
        }
    }
    
    pUnit->a = aJoin;
    pUnit->n = Num(aJoin, pJoin);
    return pJoin;
}



template <class T>
T* Join(T* aJoin, Unit<T>* pUnit, Unit<T>* pUnit0, Unit<T>* pUnit1)
{
    auto pJoin = aJoin;
    auto p0 = pUnit0->a;
    auto p1 = pUnit1->a;
    auto n0 = pUnit0->n;
    auto n1 = pUnit1->n;
    auto v0 = std::move(*p0);
    auto v1 = std::move(*p1);
    
    while (true){
        if (v1 < v0){
            *pJoin++ = std::move(v1);
            if (--n1){
                v1 = std::move(*++p1);
            } else {
                pJoin = Copy(pJoin, p0, n0, v0);
                break;
            }
        } else {
            *pJoin++ = std::move(v0);
            if (--n0){
                v0 = std::move(*++p0);
            } else {
                pJoin = Copy(pJoin, p1, n1, v1);
                break;
            }
        }
    }
    
    pUnit->a = aJoin;
    pUnit->n = Num(aJoin, pJoin);
    return pJoin;
}



template <class T>
T* InitPart(Part<T>* pPart, T* pSrc, T* eSrc, T** paDsc)
{
    auto aAsc = pSrc;
    auto eAsc = aAsc;
    
    {   // 
        Auto nSrc = Num(pSrc, eSrc);
        
        auto aIns = &pSrc[0];
        auto eIns = &pSrc[(nSrc < nIns)? nSrc: nIns];
        while (++eAsc < eIns){
            if (eAsc[0] < eAsc[-1]){
                auto pIns = eAsc;
                auto v = std::move(pIns[0]);
                do {
                    pIns[0] = std::move(pIns[-1]);
                } while (--pIns > aIns && v < pIns[-1]);
                pIns[0] = std::move(v);
            }
        }
        
        for (; (eAsc < eSrc) && !(eAsc[0] < eAsc[-1]); ++eAsc);
    }
    
    {   // 
        auto aDsc = *paDsc;
        auto eDsc = aDsc;
        
        auto pOdd = eAsc;
        auto nOdd = Num(pOdd, eSrc);
        if (nOdd){
            if (*pOdd < aAsc[0]/*Min*/){
                *--aDsc = std::move(*pOdd++);
                
                while (--nOdd){
                    if (*pOdd < eAsc[-1]/*Max*/){
                        if (*pOdd < aDsc[0]/*Min*/){
                            *--aDsc = std::move(*pOdd++);
                        } else {
                            break;
                        }
                    } else {
                        *eAsc++ = std::move(*pOdd++);
                    }
                }
            }
        }
        
        {   // 
            Auto nDsc = Num(aDsc, eDsc);
            pPart->a[Part<T>::oUnit_Asc] = aAsc;
            pPart->n[Part<T>::oUnit_Asc] = Num(aAsc, eAsc);
            pPart->a[Part<T>::oUnit_Dsc] = aDsc;
            pPart->n[Part<T>::oUnit_Dsc] = nDsc;
            pPart->o = (nDsc)? Part<T>::oUnit_Dsc: Part<T>::oUnit_Asc;
        }
        
        *paDsc = aDsc;
        return (nOdd)? pOdd: nullptr;
    }
}
}



// 

template <class T> void Sort(T* const aSrc, std::size_t nSrc, T* const aExt = nullptr);



// 

template <class T>
void Sort(T* const aSrc, std::size_t nSrc, T* const aExt)
{
    using namespace Private;
    
    if (aSrc && nSrc > 1){
        auto aTmp = (aExt)? aExt: new T[nSrc];
        
        Auto nDive = LowerLimit((MsbAlignment(nSrc) - bIns), 1);
        Auto aDive = local_array(Dive<T>, (nDive+1));
        for (int oDive = 0; oDive < nDive; ++oDive) aDive[oDive].mpJoin = (oDive & Bit(0))? aTmp: aSrc;
        
        {   // 
            std::size_t nJoin = 0;
            
            {   // 
                auto pJoin = aTmp;
                
                auto pSrc = &aSrc[0];
                auto eSrc = &aSrc[nSrc];
                while (pSrc){
                    Unit<T> vUnit;
                    
                    {   // 
                        Part<T> vPart0, vPart1;
                        auto aDsc = &aTmp[nSrc];
                        if ((pSrc = InitPart(&vPart0, pSrc, eSrc, &aDsc))){
                            pSrc = InitPart(&vPart1, pSrc, eSrc, &aDsc);
                            pJoin = Join(pJoin, &vUnit, &vPart0, &vPart1);
                        } else {
                            if (vPart0.n[Part<T>::oUnit_Asc] < nSrc){
                                pJoin = Join(pJoin, &vUnit, &vPart0);
                            } else {
                                break;
                            }
                        }
                    }
                    
                    {   // 
                        auto pDive = &aDive[0];
                        auto Carry = nJoin++;
                        Carry = (nJoin ^ Carry) & Carry;
                        for (; Carry; Carry >>= 1, ++pDive){
                            pDive->mpJoin = Join(pDive->mpJoin, &vUnit, &pDive->mUnit, &vUnit);
                        }
                        pDive->mUnit = vUnit;
                    }
                }
            }
            
            if (nJoin){
                auto bJoin = nJoin & -nJoin;
                auto oDive = Msb(bJoin);
                auto pResult = &aDive[oDive++];
                
                {   // 
                    auto pDive = &aDive[oDive];
                    auto Carry = nJoin ^ bJoin;
                    Carry >>= oDive;
                    for (; Carry; Carry >>= 1, ++pDive){
                        if (Carry & Bit(0)){
                            Join(pDive->mpJoin, &pResult->mUnit, &pDive->mUnit, &pResult->mUnit);
                        }
                    }
                }
                
                if (pResult->mUnit.a == aTmp){
                    Copy(aSrc, pResult->mUnit.a, pResult->mUnit.n);
                }
            }
        }
        
        if (aExt == nullptr) delete[] aTmp;
    }
}
}



#undef Auto
#undef local_alloc
#undef local_array
