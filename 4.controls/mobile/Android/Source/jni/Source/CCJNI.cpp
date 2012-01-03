/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright � 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCJNI.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCJNI.h"

#include "CCGLView.h"


void CCJNI::Assert(const char *file, const int line, const char *message)
{
	// JNI Java call
	JNIEnv *env = gView->jniEnv;
	jobject obj = gView->jniObj;
	jclass jniClass = env->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	// Get the method ID of our method "urlRequest", which takes one parameter of type string, and returns void
	static jmethodID mid = env->GetMethodID( jniClass, "Assert", "(Ljava/lang/String;ILjava/lang/String;)V" );
	ASSERT( mid != 0 );

	// Call the function
	jstring jFile = env->NewStringUTF( file );
	jstring jMessage = env->NewStringUTF( message );
	env->CallVoidMethod( obj, mid, jFile, line, jMessage );
}


void CCJNI::AdvertsToggle(const bool toggle)
{
	// JNI Java call
	JNIEnv *env = gView->jniEnv;
	jobject obj = gView->jniObj;
	jclass jniClass = env->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	// Get the method ID of our method "urlRequest", which takes one parameter of type string, and returns void
	static jmethodID mid = env->GetMethodID( jniClass, "advertsToggle", "(Z)V" );
	ASSERT( mid != 0 );

	// Call the function
	env->CallVoidMethod( obj, mid, toggle );
}


extern "C"
{
    JNIEXPORT void JNICALL Java_com_android2c_CCJNI_webViewURLLoaded(JNIEnv *jEnv, jobject jObj, jstring jUrl);
};

JNIEXPORT void JNICALL Java_com_android2c_CCJNI_webViewURLLoaded(JNIEnv *jEnv, jobject jObj, jstring jUrl)
{
	gView->jniEnv = jEnv;
	gView->jniObj = jObj;

	// Convert the strings
	jboolean isCopy;
	const char *cUrl = jEnv->GetStringUTFChars( jUrl, &isCopy );

	jEnv->ReleaseStringUTFChars( jUrl, cUrl );
}


void CCJNI::WebViewOpen()
{
	// JNI Java call
	JNIEnv *env = gView->jniEnv;
	jobject obj = gView->jniObj;
	jclass jniClass = env->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	// Get the method ID of our method "urlRequest", which takes one parameter of type string, and returns void
	static jmethodID mid = env->GetMethodID( jniClass, "webViewOpen", "(Ljava/lang/String;)V" );
	ASSERT( mid != 0 );

	// Call the function
	jstring javaURL = env->NewStringUTF( "http://www.google.com" );
	env->CallVoidMethod( obj, mid, javaURL );
}


void CCJNI::WebViewClose()
{
	// JNI Java call
	JNIEnv *env = gView->jniEnv;
	jobject obj = gView->jniObj;

	jclass jniClass = env->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	// Get the method ID of our method "urlRequest", which takes one parameter of type string, and returns void
	static jmethodID mid = env->GetMethodID( jniClass, "webViewClose", "()V" );
	ASSERT( mid != 0 );

	// Call the function
	env->CallVoidMethod( obj, mid );
}


void CCJNI::WebViewClearData()
{
	// JNI Java call
	JNIEnv *env = gView->jniEnv;
	jobject obj = gView->jniObj;

	jclass jniClass = env->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	// Get the method ID of our method "urlRequest", which takes one parameter of type string, and returns void
	static jmethodID mid = env->GetMethodID( jniClass, "webViewClearData", "()V" );
	ASSERT( mid != 0 );

	// Call the function
	env->CallVoidMethod( obj, mid );
}



void CCJNI::VideoViewStart(const char *file)
{
	// JNI Java call
	JNIEnv *jEnv = gView->jniEnv;
	jobject jObj = gView->jniObj;

	jclass jniClass = jEnv->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	// Get the method ID of our method "startVideoView", which takes one parameter of type string, and returns void
	static jmethodID mid = jEnv->GetMethodID( jniClass, "videoViewStart", "(Ljava/lang/String;)V" );
	ASSERT( mid != 0 );

	// Call the function
	jstring javaURL = jEnv->NewStringUTF( file );
	jEnv->CallVoidMethod( jObj, mid, javaURL );
}


void CCJNI::VideoViewStop()
{
	// JNI Java call
	JNIEnv *env = gView->jniEnv;
	jobject obj = gView->jniObj;
	jclass jniClass = env->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	// Get the method ID of our method "stopVideoView", which takes 0 parameters, and returns void
	static jmethodID mid = env->GetMethodID( jniClass, "videoViewStop", "()V" );
	ASSERT( mid != 0 );

	// Call the function
	env->CallVoidMethod( obj, mid );
}
