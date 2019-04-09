#pragma once
// Copyright(c) 2018 Emura Daisuke
// MIT License



#include <cstddef>
#include <utility>
#include <vector>
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



// 

template <class RandomAccessIterator>
inline void Sort(RandomAccessIterator const first, RandomAccessIterator const last);

template <class RandomAccessIterator, class Compare>
inline void Sort(RandomAccessIterator const first, RandomAccessIterator const last, Compare comp);



// 

template <class RandomAccessIterator, class Compare> class Private
{
    using itr_t = RandomAccessIterator;
    using val_t = typename std::iterator_traits<itr_t>::value_type;
    using ref_t = typename std::iterator_traits<itr_t>::reference;
    using dif_t = typename std::iterator_traits<itr_t>::difference_type;
    using cmp_t = Compare;
    
    
    
    private:
        static constexpr std::size_t Bit(int v) noexcept
        {
            return (1ULL << v);
        }
        
        
        
        static constexpr std::size_t cbIns = 5;
        static constexpr std::size_t cnIns = Bit(cbIns);
        
        
        
        struct Part
        {
            enum oUnit {
                oUnit_Asc,
                oUnit_Dsc,
                oUnit_Num,
            };
            
            itr_t   a[oUnit_Num];
            dif_t   n[oUnit_Num];
            oUnit   o;
        };
        
        
        
        struct Unit
        {
            itr_t   a;
            dif_t   n;
        };
        
        
        
        struct Dive
        {
            Unit    mUnit;
            itr_t   mpJoin;
        };
    
    
    
    private:
        static int Msb(std::size_t v) noexcept
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
        
        
        
        static int MsbAlignment(std::size_t v) noexcept
        {
            return Msb(v + v - 1);
        }
        
        
        
        static int LowerLimit(int v, int limit) noexcept
        {
            return (v > limit)? v: limit;
        }
        
        
        
        static dif_t Num(itr_t const first, itr_t const last) noexcept
        {
            return last - first;
        }
        
        
        
        static itr_t Copy(itr_t iDst, itr_t iSrc, dif_t nSrc)
        {
            while (nSrc--) *iDst++ = std::move(*iSrc++);
            return iDst;
        }
        
        
        
        static itr_t Copy(itr_t iDst, itr_t iSrc, dif_t nSrc, ref_t v)
        {
            *iDst = std::move(v);
            return Copy(++iDst, ++iSrc, --nSrc);
        }
        
        
        
        static itr_t Join(itr_t iJoin, Unit& rUnit, const Part& rPart)
        {
            auto nDsc = rPart.n[Part::oUnit_Dsc];
            auto nAsc = rPart.n[Part::oUnit_Asc];
            
            rUnit.a = iJoin;
            rUnit.n = nDsc + nAsc;
            
            iJoin = Copy(iJoin, rPart.a[Part::oUnit_Dsc], nDsc);
            iJoin = Copy(iJoin, rPart.a[Part::oUnit_Asc], nAsc);
            return iJoin;
        }
        
        
        
        static itr_t Join(itr_t aJoin, Unit& rUnit, const Part& rPart0, const Part& rPart1, const cmp_t& Comp)
        {
            auto iJoin = aJoin;
            auto o0 = rPart0.o;
            auto o1 = rPart1.o;
            auto i0 = rPart0.a[o0];
            auto i1 = rPart1.a[o1];
            auto n0 = rPart0.n[o0];
            auto n1 = rPart1.n[o1];
            auto v0 = std::move(*i0);
            auto v1 = std::move(*i1);
            
            while (true){
                if (Comp(v1, v0)){
                    *iJoin++ = std::move(v1);
                    if (--n1){
                        v1 = std::move(*++i1);
                    } else {
                        if (o1){
                            o1 = Part::oUnit_Asc;
                            i1 = rPart1.a[o1];
                            n1 = rPart1.n[o1];
                            v1 = std::move(*i1);
                        } else {
                            iJoin = Copy(iJoin, i0, n0, v0);
                            if (o0) iJoin = Copy(iJoin, rPart0.a[Part::oUnit_Asc], rPart0.n[Part::oUnit_Asc]);
                            break;
                        }
                    }
                } else {
                    *iJoin++ = std::move(v0);
                    if (--n0){
                        v0 = std::move(*++i0);
                    } else {
                        if (o0){
                            o0 = Part::oUnit_Asc;
                            i0 = rPart0.a[o0];
                            n0 = rPart0.n[o0];
                            v0 = std::move(*i0);
                        } else {
                            iJoin = Copy(iJoin, i1, n1, v1);
                            if (o1) iJoin = Copy(iJoin, rPart1.a[Part::oUnit_Asc], rPart1.n[Part::oUnit_Asc]);
                            break;
                        }
                    }
                }
            }
            
            rUnit.a = aJoin;
            rUnit.n = Num(aJoin, iJoin);
            return iJoin;
        }
        
        
        
        static itr_t Join(itr_t aJoin, Unit& rUnit, const Unit& rUnit0, const Unit& rUnit1, const cmp_t& Comp)
        {
            auto iJoin = aJoin;
            auto i0 = rUnit0.a;
            auto i1 = rUnit1.a;
            auto n0 = rUnit0.n;
            auto n1 = rUnit1.n;
            auto v0 = std::move(*i0);
            auto v1 = std::move(*i1);
            
            while (true){
                if (Comp(v1, v0)){
                    *iJoin++ = std::move(v1);
                    if (--n1){
                        v1 = std::move(*++i1);
                    } else {
                        iJoin = Copy(iJoin, i0, n0, v0);
                        break;
                    }
                } else {
                    *iJoin++ = std::move(v0);
                    if (--n0){
                        v0 = std::move(*++i0);
                    } else {
                        iJoin = Copy(iJoin, i1, n1, v1);
                        break;
                    }
                }
            }
            
            rUnit.a = aJoin;
            rUnit.n = Num(aJoin, iJoin);
            return iJoin;
        }
        
        
        
        static bool MakePart(Part& rPart, itr_t& riSrc, itr_t eSrc, itr_t& raDsc, const cmp_t& Comp)
        {
            auto iSrc = riSrc;
            auto aAsc = iSrc;
            auto eAsc = aAsc;
            
            {   // 
                Auto nSrc = Num(iSrc, eSrc);
                auto nIns = (nSrc < cnIns)? nSrc: cnIns;
                
                auto aIns = iSrc;
                auto eIns = iSrc + nIns;
                while (++eAsc != eIns){
                    if (Comp(eAsc[0], eAsc[-1])){
                        auto iIns = eAsc;
                        auto v = std::move(iIns[0]);
                        do {
                            iIns[0] = std::move(iIns[-1]);
                        } while (--iIns != aIns && Comp(v, iIns[-1]));
                        iIns[0] = std::move(v);
                    }
                }
                
                for (; (eAsc != eSrc) && !Comp(eAsc[0], eAsc[-1]); ++eAsc);
            }
            
            {   // 
                auto aDsc = raDsc;
                auto eDsc = aDsc;
                
                auto iOdd = eAsc;
                auto nOdd = Num(iOdd, eSrc);
                if (nOdd){
                    if (Comp(*iOdd, aAsc[0]/*Min*/)){
                        *--aDsc = std::move(*iOdd++);
                        
                        while (--nOdd){
                            if (Comp(*iOdd, eAsc[-1]/*Max*/)){
                                if (Comp(*iOdd, aDsc[0]/*Min*/)){
                                    *--aDsc = std::move(*iOdd++);
                                } else {
                                    break;
                                }
                            } else {
                                *eAsc++ = std::move(*iOdd++);
                            }
                        }
                    }
                }
                
                {   // 
                    Auto nDsc = Num(aDsc, eDsc);
                    rPart.a[Part::oUnit_Asc] = aAsc;
                    rPart.n[Part::oUnit_Asc] = Num(aAsc, eAsc);
                    rPart.a[Part::oUnit_Dsc] = aDsc;
                    rPart.n[Part::oUnit_Dsc] = nDsc;
                    rPart.o = (nDsc)? Part::oUnit_Dsc: Part::oUnit_Asc;
                }
                
                riSrc = iOdd;
                raDsc = aDsc;
                return (nOdd);
            }
        }
        
        
        
        static void Turn(itr_t iDst, const Part& rPart)
        {
            auto nDsc = rPart.n[Part::oUnit_Dsc];
            auto nAsc = rPart.n[Part::oUnit_Asc];
            auto aDsc = iDst;
            auto aAsc = iDst + nDsc;
            
            if (nDsc){
                Copy(aAsc, rPart.a[Part::oUnit_Asc], nAsc);
                Copy(aDsc, rPart.a[Part::oUnit_Dsc], nDsc);
            }
        }
    
    
    
    public:
        static void Sort(itr_t const first, itr_t const last, cmp_t comp)
        {
            Auto nSrc = Num(first, last);
            if (nSrc > 1){
                std::vector<val_t> aVal(nSrc);
                
                Auto aSrc = first;
                Auto eSrc = last;
                Auto aTmp = aVal.begin();
                Auto eTmp = aVal.end();
                
                Auto nDive = LowerLimit((MsbAlignment(nSrc) - cbIns), 1);
                Auto aDive = local_array(Dive, (nDive+1));
                for (int oDive = 0; oDive < nDive; ++oDive) aDive[oDive].mpJoin = (oDive & Bit(0))? aTmp: aSrc;
                
                {   // 
                    std::size_t nJoin = 0;
                    
                    {   // 
                        auto iJoin = aTmp;
                        
                        auto iSrc = aSrc;
                        while (iSrc != eSrc){
                            Unit vUnit;
                            
                            {   // 
                                Part vPart0, vPart1;
                                auto aDsc = eTmp;
                                if (MakePart(vPart0, iSrc, eSrc, aDsc, comp)){
                                    MakePart(vPart1, iSrc, eSrc, aDsc, comp);
                                    iJoin = Join(iJoin, vUnit, vPart0, vPart1, comp);
                                } else {
                                    if (nJoin){
                                        iJoin = Join(iJoin, vUnit, vPart0);
                                    } else {
                                        Turn(aSrc, vPart0);
                                        break;
                                    }
                                }
                            }
                            
                            {   // 
                                auto pDive = &aDive[0];
                                auto Carry = nJoin++;
                                Carry = (nJoin ^ Carry) & Carry;
                                for (; Carry; Carry >>= 1, ++pDive){
                                    pDive->mpJoin = Join(pDive->mpJoin, vUnit, pDive->mUnit, vUnit, comp);
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
                                    Join(pDive->mpJoin, pResult->mUnit, pDive->mUnit, pResult->mUnit, comp);
                                }
                            }
                        }
                        
                        if (pResult->mUnit.a == aTmp){
                            Copy(aSrc, pResult->mUnit.a, pResult->mUnit.n);
                        }
                    }
                }
            }
        }
};



template <class RandomAccessIterator>
inline void Sort(RandomAccessIterator const first, RandomAccessIterator const last)
{
    HayateShiki::Sort(first, last, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}



template <class RandomAccessIterator, class Compare>
inline void Sort(RandomAccessIterator const first, RandomAccessIterator const last, Compare comp)
{
    HayateShiki::Private<RandomAccessIterator, Compare>::Sort(first, last, comp);
}
}



#undef Auto
#undef local_alloc
#undef local_array
