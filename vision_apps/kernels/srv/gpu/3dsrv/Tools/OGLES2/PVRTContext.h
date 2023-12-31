/*!****************************************************************************

 @file         OGLES2/PVRTContext.h
 @ingroup      API_OGLES2
 @copyright    Copyright (c) Imagination Technologies Limited.
 @brief        Context specific stuff - i.e. 3D API-related.

******************************************************************************/

#ifndef _PVRTCONTEXT_H_
#define _PVRTCONTEXT_H_

/*!
 @addtogroup   API_OGLES2
 @{
*/

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <stdio.h>
#if defined(BUILD_OGLES2)
#if defined(__APPLE__)
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE==1
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#else	//OSX 
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
#else
#if defined(__PALMPDK__)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#if !defined(EGL_NOT_PRESENT)
#include <EGL/egl.h>
#endif
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
#endif
#elif defined(BUILD_OGLES3) 
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE==1
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#else
#include <EGL/egl.h>
#if defined(BUILD_OGLES31)
	#include <GLES3/gl31.h>
#else
	#include <GLES3/gl3.h>
#endif
#include <GLES2/gl2ext.h>
#endif
#endif

/****************************************************************************
** Macros
****************************************************************************/
#define PVRTRGBA(r, g, b, a)   ((GLuint) (((a) << 24) | ((b) << 16) | ((g) << 8) | (r)))

/****************************************************************************
** Defines
****************************************************************************/

/****************************************************************************
** Enumerations
****************************************************************************/

/****************************************************************************
** Structures
****************************************************************************/

/*!**************************************************************************
 @struct    SPVRTContext
 @brief     A structure for storing API specific variables
****************************************************************************/
struct SPVRTContext
{
	int reserved;	// No context info for OGLES2.
};

/****************************************************************************
** Functions
****************************************************************************/

/*! @} */

#endif /* _PVRTCONTEXT_H_ */

/*****************************************************************************
 End of file (PVRTContext.h)
*****************************************************************************/

