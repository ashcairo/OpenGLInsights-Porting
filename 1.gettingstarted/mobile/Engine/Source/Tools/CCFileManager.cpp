/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCFileManager.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCDeviceFileManager.h"

#ifdef Q_OS_WIN
    #include <io.h>
#endif


// Must provide full path
const int CCFileManager::getFileData(const char *fullFilePath, char **data, CCResourceType resourceType, const bool assertOnFail)
{
    CCDeviceFileManager *file = new CCDeviceFileManager( resourceType );
    if( file->open( fullFilePath ) == false )
    {
        if( assertOnFail )
        {
            DEBUGLOG( "CCFileManager::getFileData( %s )\n", fullFilePath );
            ASSERT( false );
        }
        delete file;
        return -1;
    }

    const uint fileSize = file->size();
    *data = (char*)malloc( fileSize+1 );
    file->read( *data, fileSize );
    file->close();
    delete file;

    // Add null terminate character to the end
    (*data)[fileSize] = 0;
    
    return fileSize;
}


void CCFileManager::getFilePath(CCText &fullFilePath, const char *filePath, CCResourceType resourceType)
{
#ifdef QT

    fullFilePath = QtRootPath().toUtf8().constData();
    fullFilePath += filePath;

#elif defined( IOS )

    CCText deviceFilePath( filePath );
    deviceFilePath.stripDirectory();

    if( resourceType == Resource_Packaged )
    {
        fullFilePath = [[[NSBundle mainBundle] resourcePath] UTF8String];
        fullFilePath += "/";
    }
    else
    {
        CCText docsDirectory = CCDeviceFileManager::GetDocsFolder();
        fullFilePath = docsDirectory.buffer;
    }

    fullFilePath += deviceFilePath.buffer;

#elif defined( ANDROID )

    CCText filename;
    filename = filePath;
    filename.stripDirectory();
    filename.toLowercase();

	if( resourceType == Resource_Packaged )
	{
		fullFilePath = filename.buffer;
	}
	else
	{
		fullFilePath = CCDeviceFileManager::dataPath.buffer;
		fullFilePath += filename.buffer;
	}

#endif
}


// Must provide relative path for files packaged with the app
const int CCFileManager::getPackagedFile(const char *filePath, char **data, const bool assertOnFail, struct stat *info)
{
    CCText fullFilePath;
    getFilePath( fullFilePath, filePath, Resource_Packaged );
    
    if( info != NULL )
    {
        stat( fullFilePath.buffer, info );
    }
    
    return getFileData( fullFilePath.buffer, data, Resource_Packaged, assertOnFail );
}


// Must provide relative path for files generated/downloaded by the app
const int CCFileManager::getCachedFile(const char *filePath, char **data, const bool assertOnFail, struct stat *info)
{
    CCText fullFilePath;
    getFilePath( fullFilePath, filePath, Resource_Cached );

    if( info != NULL )
    {
        stat( fullFilePath.buffer, info );
    }

    return getFileData( fullFilePath.buffer, data, Resource_Cached, assertOnFail );
}


const bool CCFileManager::saveCachedFile(const char *filePath, const char *data, const int length)
{
#ifdef QT
    
    CCText fullFilePath = QtRootPath().toUtf8().constData();
    fullFilePath += filePath;

    QFile file( fullFilePath.buffer );
    if( file.open( QIODevice::WriteOnly ) )
    {
        file.write( data, length );
        file.close();
        
        return true;
    }
    
#elif defined( IOS ) || defined( ANDROID )
    
    CCText fullFilePath;
    getFilePath( fullFilePath, filePath, Resource_Cached );

    //DEBUGLOG( "CCFileManager::Saving %s \n", fullFilePath.buffer );
    FILE *pFile = fopen( fullFilePath.buffer, "w" );
    ASSERT( pFile != NULL );
    if( pFile != NULL )
    {
        fwrite( data, sizeof( char ), length, pFile );
        fclose( pFile );
        
        return true;
    }

#endif
    
    return false;
}


const bool CCFileManager::deleteCachedFile(const char *filePath)
{
#ifdef QT
    
    if( QFile::remove( filePath ) == false )
    {
        ASSERT( false );
        return false;
    }
    
#elif defined( IOS ) || defined( ANDROID )
    
    CCText fullFilePath;
    getFilePath( fullFilePath, filePath, Resource_Cached );
    
    //DEBUGLOG( "CCFileManager::deleteCachedFile %s \n", fullFilePath.buffer );
    if( remove( fullFilePath.buffer ) != 0 )
    {
        ASSERT( false );
        return false;
    }
    
#else
    
    ASSERT( false );
    return false;
    
#endif
    
    return true;
}


class IOPriorityCallback : public CCLambdaCallback
{
public:
    IOPriorityCallback(const int inPriority)
    {
        priority = inPriority;
    }
    int priority;
};


static CCList<IOPriorityCallback> pendingIO;
static int numberOfIORequests = 0;
#define MAX_IO_PER_FRAME 3
void CCFileManager::readyIO()
{
    numberOfIORequests = 0;

    while( pendingIO.length > 0 )
    {
        if( numberOfIORequests < MAX_IO_PER_FRAME )
        {
            IOPriorityCallback *ioCallback = pendingIO.list[0];
            pendingIO.remove( ioCallback );
            ioCallback->run();
            delete ioCallback;
        }
        else
        {
            break;
        }
    }
    
#if 0 && defined DEBUGON
    static int currentHighPriorityIO = 0;
    static int maxHighPrirityIO = 0;
    for( int i=0; i<pendingIO.length; ++i )
    {
        IOPriorityCallback *ioCallback = pendingIO.list[i];
        if( ioCallback->priority > 0 )
        {
            currentHighPriorityIO = i;
        }
    }
    LOG_NEWMAX( "Highest Priority IO remaining", maxHighPrirityIO, currentHighPriorityIO );
#endif
}


void CCFileManager::doesCachedFileExistAsync(const char *filePath, CCIOCallback *inCallback)
{
    inCallback->filePath = filePath;

    class FileExistsCallback : public IOPriorityCallback
    {
    public:
        FileExistsCallback(CCIOCallback *inCallback) :
            IOPriorityCallback( inCallback->priority )
        {
            this->ioCallback = inCallback;
        }

        void run()
        {
            if( ioCallback->isCallbackActive() == false )
            {
                return;
            }

            ASSERT( priority == ioCallback->priority );

            CCText fullFilePath;
            getFilePath( fullFilePath, ioCallback->filePath.buffer, Resource_Cached );

            numberOfIORequests++;
            const int result = access( fullFilePath.buffer, F_OK );
            ioCallback->exists = result == 0;

            ASSERT( priority == ioCallback->priority );

            ioCallback->run();
        }

    private:
        CCIOCallback *ioCallback;
    };


    FileExistsCallback *fileExistsCallback = new FileExistsCallback( inCallback );
    if( /*fileExistsCallback->priority > 0 && */ numberOfIORequests < MAX_IO_PER_FRAME )
    {
        fileExistsCallback->run();
        delete fileExistsCallback;
    }
    else
    {
        pendingIO.add( fileExistsCallback );
        
        // Can't do this as it'll mess up the order they we're requested
//        for( int i=0; i<pendingIO.length; ++i )
//        {
//            IOPriorityCallback *ioRequest = pendingIO.list[i];
//            if( ioRequest->priority < fileExistsCallback->priority )
//            {
//                pendingIO.reinsert( fileExistsCallback, i );
//                break;
//            }
//        }
    }
}


const bool CCFileManager::doesCachedFileExist(const char *filePath)
{
    CCText fullFilePath;
    getFilePath( fullFilePath, filePath, Resource_Cached );

    const int result = access( fullFilePath.buffer, F_OK );
    return result == 0;
}
