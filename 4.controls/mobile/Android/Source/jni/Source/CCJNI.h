/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright � 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCJNI.h
 * Description : C++ JNI interface class to CCJNI.java
 *
 * Created     : 08/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCJNI_H__
#define __CCJNI_H__


class CCJNI
{
public:
	static void Assert(const char *file, const int line, const char *message);

	static void AdvertsToggle(const bool toggle);

	static void WebViewOpen();
	static void WebViewClose();
	static void WebViewClearData();

	static void VideoViewStart(const char *file);
	static void VideoViewStop();
};


#endif // __CCJNI_H__
