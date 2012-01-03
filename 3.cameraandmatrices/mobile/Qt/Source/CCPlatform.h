/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCPlatform.h
 * Description : Platform specific functions.
 *
 * Created     : 25/01/11
 * Author(s)   : Jerome Portier, Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCPLATFORM_H__
#define __CCPLATFORM_H__


#define QT

#include <QtGlobal>
#include <QtCore/QEvent>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtCore/QTime>
#include <QtCore/QString>

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/qglshaderprogram.h>

#include <unistd.h>

#define _USE_MATH_DEFINES
#include <math.h>
#define MAX(x,y)	((x>=y)?x:y)
#define MIN(x,y)	((x>=y)?y:x)
#define MAXFLOAT 	((float)3.40282346638528860e+38)

// OpenGL
#define glClearDepthf   glClearDepth
#define GL_BGRA         0x80E1


extern void CCEngineThreadLock();
extern void CCEngineThreadUnlock();


const QString QtRootPath();


#endif // __CCPLATFORM_H__
