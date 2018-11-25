#pragma	once
// Copyright(c) Emura Daisuke. All rights reserved.



#include <malloc.h>

#if _MSC_VER//[
// msvc
#define	alloca		_alloca
#define	Continue	// nop
#elif __clang__//][
// clang
#define	Continue	continue
#else//][
// gcc
#define	Continue	// nop
#endif//]

#define	Auto				decltype(auto)
#define	LocalAlloc(T, s)	static_cast<T*>(alloca(s))

#include "./qtq.Merge.TypeV.h"
