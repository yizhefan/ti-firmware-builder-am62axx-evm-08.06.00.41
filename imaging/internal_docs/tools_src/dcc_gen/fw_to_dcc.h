#ifndef __FRAMEWORK_TO_DCC_H__
#define __FRAMEWORK_TO_DCC_H__

//This file should be present in the dcc include path in order to be used by the
//generated parser sources.

//These are the includes necessary from the framework library in order to have
//functionality in the generated dcc parser file.

// These should be changed according to the build environment

//#include "/path/to/ctypes.h"
//#include "osal/stdtypes.h"
//#include "ctypes.h"
//#include "/path/to/osal.h"
//#include "tools.h"
#include <stdlib.h>

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;
typedef signed char  int8;
typedef signed short int16;
typedef signed long  int32;

#define printf_dbg mmsdbg
//#define printf_dbg printf

#define dcc_prs_malloc(aSize)                 malloc(aSize)
#define dcc_prs_free(ptr)                     free(ptr)

#endif // __FRAMEWORK_TO_DCC_H__
