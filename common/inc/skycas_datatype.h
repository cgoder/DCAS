#ifndef __SKYCAS_DATATYPES_H__
#define __SKYCAS_DATATYPES_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif     /* __cplusplus */
#endif  /* __cplusplus */

#include "stdio.h"
#include "string.h"
typedef unsigned char SKYCAS_U8;
typedef unsigned char SKYCAS_UCHAR;
typedef unsigned short SKYCAS_U16;
typedef unsigned int SKYCAS_U32;
typedef unsigned long SKYCAS_LONG;

typedef signed char SKYCAS_S8;
typedef short SKYCAS_S16;
typedef int SKYCAS_S32;

typedef unsigned long long SKYCAS_U64;
typedef long long SKYCAS_S64;

typedef char SKYCAS_CHAR;
typedef char* SKYCAS_PCHAR;

typedef float SKYCAS_FLOAT;
typedef double SKYCAS_DOUBLE;

typedef enum
{
    SKYCAS_FALSE    = 0,
    SKYCAS_TRUE     = 1,
}SKYCAS_BOOL;

#define SKYCAS_VOID void

#ifndef SKYCAS_NULL
#define SKYCAS_NULL             (0L)
#endif

#define SKYCAS_RET_SUCCESS       (0)
#define SKYCAS_RET_FAILURE       (-1)














#ifdef __cplusplus
 #if __cplusplus
	}
 #endif     /* __cplusplus */
#endif  /* __cplusplus */
	
#endif /* __SKYCAS_DATATYPES_H__ */

