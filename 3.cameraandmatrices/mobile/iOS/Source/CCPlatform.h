/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCPlatform.h
 * Description : Platform specific functions.
 *
 * Created     : 25/01/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCPLATFORM_H__
#define __CCPLATFORM_H__
 

#define IOS

#ifdef __OBJC__

#include <OpenGLES/EAGL.h>

#else

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <cctype>

#include <math.h>
#define MAX(x,y) ((x>=y)?x:y)
#define MIN(x,y) ((x>=y)?y:x)

typedef	unsigned int uint;

#endif

//#import <OpenGLES/ES1/gl.h>
//#import <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

//#define _USE_MATH_DEFINES
//#include <math.h>
//
//typedef unsigned int uint;
//
////#include <stdio.h>
//#include <stdlib.h>
//
//#ifndef MAX
//#define MAX(x,y)	((x>=y)?x:y)
//#endif
//
//#ifndef MIN
//#define MIN(x,y)	((x>=y)?y:x)
//#endif

extern void CCEngineThreadLock();
extern void CCEngineThreadUnlock();


#endif // __CCPLATFORM_H__