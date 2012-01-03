/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCPlatform.cpp
 *-----------------------------------------------------------
 */

#include "CCPlatform.h"
#include "CCBaseTypes.h"


void CCEngineThreadLock()
{
}


void CCEngineThreadUnlock()
{
}



#include <QtCore/QCoreApplication>

const QString QtRootPath()
{
    QString path = QCoreApplication::applicationDirPath();
    
#ifdef Q_OS_WIN

    CCText runPath;
    CCText buildPath = path.toUtf8().constData();
    CCList<char> *directorySplit = buildPath.split( "/" );
    for( int i=0; i<directorySplit->length; ++i )
    {
        const char *buffer = directorySplit->list[i];
        runPath += buffer;
        runPath += "/";

        if( CCText::Contains( buffer, "-build-desktop" ) )
        {
            break;
        }
    }
    DELETE_POINTER( directorySplit );

    path = runPath.buffer;
    
    return path;
    
#elif defined Q_OS_MACX
    
    return path + "/";
    
#elif defined Q_OS_SYMBIAN
    
    return path;
    
#else // MeeGo?
    
    return QString( "/opt/lambis" );
    
#endif
}
