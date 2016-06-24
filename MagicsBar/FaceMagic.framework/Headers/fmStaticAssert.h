/*
@Author: wysaid
@Blog: blog.wysaid.org
@Date: 2013-10-31
*/

#ifndef _FM_STATICASSERT_H_
#define _FM_STATICASSERT_H_

#ifndef _FM_STATIC_ASSERT_

#define fmStaticAssert(value) static_assert(value, "Invalid Parameters!")

#else

#if defined(DEBUG) || defined(_DEBUG)

template<bool K>
struct _FMStaticAssert ;

template<>
struct _FMStaticAssert<true> { int dummy; };

template<int n>
struct __FMStaticAssert {};

#define fmStaticAssert(value) do \
{\
	typedef __FMStaticAssert<\
	sizeof(_FMStaticAssert<(bool)(value)>)\
	> ___FMStaticAssert;\
} while (0)

#else

#define fmStaticAssert(...) 

#endif

#endif

#endif //_FM_STATICASSERT_H_
