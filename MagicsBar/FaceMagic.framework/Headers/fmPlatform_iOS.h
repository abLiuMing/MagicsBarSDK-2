/*
 * fmPlatforms.h
 *
 *  Created on: 2013-12-31
 *      Author: Wang Yang
 *  Description: load some library and do some essential initialization before compiling.
 */

#ifndef FMPLATFORMS_H_
#define FMPLATFORMS_H_

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#if (defined(DEBUG) || defined(_DEBUG)) && !defined(_FM_IOS_NO_LOGS_)

#include <stdio.h>

#ifndef FM_LOG_INFO
#define FM_LOG_INFO(...) printf(__VA_ARGS__)
#endif

#ifndef FM_LOG_ERROR
#define FM_LOG_ERROR(str, ...) \
do{\
fprintf(stderr, "❌❌❌");\
fprintf(stderr, str, ##__VA_ARGS__);\
fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);\
fprintf(stderr, "❌❌❌\n");\
}while(0)
#endif

#ifndef FM_LOG_CODE
#define FM_LOG_CODE(...) __VA_ARGS__
#endif

#else

#ifndef FM_LOG_INFO
#define FM_LOG_INFO(...)
#endif

#ifndef FM_LOG_ERROR
#define FM_LOG_ERROR(...)
#endif

#ifndef FM_LOG_CODE
#define FM_LOG_CODE(...)
#endif

#endif

#endif /* HTPLATFORMS_H_ */
