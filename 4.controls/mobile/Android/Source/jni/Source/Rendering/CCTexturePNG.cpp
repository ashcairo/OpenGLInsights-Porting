/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright � 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTexturePNG.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCTexturePNG.h"
#include "CCTextureManager.h"
#include "CCDeviceFileManager.h"

#include "CCGLView.h"


CCTexturePNG::CCTexturePNG()
{
}


CCTexturePNG::~CCTexturePNG()
{
}


static int jniLoad(const char *name, const bool generateMipMap, const bool packaged)
{
	// JNI Java call
	JNIEnv *jEnv = gView->jniEnv;
	jobject jObj = gView->jniObj;

	jclass jniClass = jEnv->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	static jmethodID mid = jEnv->GetMethodID( jniClass, "textureLoad", "(Ljava/lang/String;ZZ)I" );
	ASSERT( mid != 0 );

	// Call the function
	jstring jFilename = jEnv->NewStringUTF( name );
	return jEnv->CallIntMethod( jObj, mid, jFilename, generateMipMap, packaged );
}


static int jniGetWidth()
{
	// JNI Java call
	JNIEnv *jEnv = gView->jniEnv;
	jobject jObj = gView->jniObj;

	jclass jniClass = jEnv->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	static jmethodID mid = jEnv->GetMethodID( jniClass, "textureGetWidth", "()I" );
	ASSERT( mid != 0 );

	// Call the function
	return jEnv->CallIntMethod( jObj, mid );
}


static int jniGetHeight()
{
	// JNI Java call
	JNIEnv *jEnv = gView->jniEnv;
	jobject jObj = gView->jniObj;

	jclass jniClass = jEnv->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	static jmethodID mid = jEnv->GetMethodID( jniClass, "textureGetHeight", "()I" );
	ASSERT( mid != 0 );

	// Call the function
	return jEnv->CallIntMethod( jObj, mid );
}


static int jniGetScaledHeight()
{
	// JNI Java call
	JNIEnv *jEnv = gView->jniEnv;
	jobject jObj = gView->jniObj;

	jclass jniClass = jEnv->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	static jmethodID mid = jEnv->GetMethodID( jniClass, "textureGetScaledHeight", "()I" );
	ASSERT( mid != 0 );

	// Call the function
	return jEnv->CallIntMethod( jObj, mid );
}


static void jniReleaseRawData()
{
	// JNI Java call
	JNIEnv *jEnv = gView->jniEnv;
	jobject jObj = gView->jniObj;

	jclass jniClass = jEnv->FindClass( "com/android2c/CCJNI" );
	ASSERT_MESSAGE( jniClass != 0, "Could not find Java class." );

	static jmethodID mid = jEnv->GetMethodID( jniClass, "textureReleaseRawData", "()V" );
	ASSERT( mid != 0 );

	// Call the function
	jEnv->CallVoidMethod( jObj, mid );
}


const bool CCTexturePNG::load(const char *path, const CCResourceType resourceType, const bool generateMipMap)
{
#if defined PROFILEON
    CCProfiler profile( "CCTexturePNG::load()" );
#endif

	CCText filename = path;
	filename.stripDirectory();
	filename.toLowercase();

	glName = jniLoad( filename.buffer, generateMipMap, resourceType == Resource_Packaged );
	if( glName > 0 )
	{
		allocatedWidth = imageWidth = jniGetWidth();
		allocatedHeight = jniGetHeight();
		imageHeight = jniGetScaledHeight();

		// TODO: pretend it's always 4 bytes on Android, look into getting actual pixel size data
		allocatedBytes = allocatedWidth * allocatedHeight * 4;

		jniReleaseRawData();
		return true;
	}
	return false;
}


const bool CCTexturePNG::createGLTexture()
{
	return true;
}
